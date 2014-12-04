#include	<stdio.h>
#include	<curses.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	"ppaddle.h"
#include	"pcourt.h"
#include	"ponglib.h"
#include	"preferee.h"

//One of these values is passed into paddle_move()
#define 	MOVE_UP -1
#define		MOVE_DOWN 1


//Data struct to store the paddle state
typedef struct {
	int top,	/*Top of the paddle*/
		bottom, /*Bottom of the paddle*/
		col, 	/*The paddle's home column*/
		height; /*The height of the paddle*/
	chtype pad_char;	/*Char to draw the paddle*/
} ppaddle;


//Static singleton paddle object
static ppaddle _paddle;

//Holds the court geometry to be used when init'ing the paddle
static ppcourt_geom court_g = {0};

//Static function prototype
static void paddle_move(int);


/*paddle_init() - Initialize the paddle object
*
*/
void paddle_init(){

	//Get the boundries of the court, and set the starting position
	court_geom( &court_g );
	if ( &court_g != (ppcourt_geom *)NULL) {
		
		int court_height = (court_g.bottom - court_g.top);
		
		_paddle.height = court_height / 3;
		_paddle.top = court_g.top + _paddle.height + 1;
		_paddle.bottom = (_paddle.top + (_paddle.height - 1) );
		_paddle.col = court_g.right;
		_paddle.pad_char = '#';
	
	}
	
	//Draw the paddle in it's initial state
	mvvline(_paddle.top, _paddle.col, _paddle.pad_char, _paddle.height); //_paddle.bottom - _paddle.top);
	refresh();

}


/*paddle_up() - move the paddle up one row.
*/
void paddle_up( ){
	
	//Only proceed if we're not already at top
	if ( _paddle.top > (court_g.top + 1) ){
		
		paddle_move( MOVE_UP );
	
	}
	
}



/*paddle_down() - move the paddle up one row.
*/
void paddle_down( ){
	
	//Only proceed if we're not already at top
	if ( _paddle.bottom < (court_g.bottom - 1) ){
		
		paddle_move( MOVE_DOWN );
	
	}
	

}



/*paddle_contact() - test whether a ball
* is in contact with the paddle.
* Args: int, int - y and x coords of the ball
*/
pad_c paddle_contact( int y, int x )
{
	int r = PAD_NO_CONTACT;
	
	if ( (y >= _paddle.top) &&
		 (y <= _paddle.bottom) ){ 	  /*It's in the vert range, cont*/
		
		if ( x == _paddle.col )       /*It's ON the paddle*/
			r = PAD_INTERCEPT;
		if ( x == (_paddle.col - 1) ) /*Normal paddle hit*/
			r = PAD_CONTACT;
	
	}
	
	return r;
	
}



/*paddle_contact() - Static method to move and draw the paddle
*
*	Args: int - the direction to move the paddle, either:
*		MOVE_UP, -1
*		MOVE_DOWN, 1
*/
static void paddle_move(int dir)
{
	//Temporarily block SIGALRM to prevent the clock/ball
	//From drawing while moving the paddle
	sigset_t sigs, prev_sigs;
	sigemptyset( &sigs );
	if ( sigaddset( &sigs, SIGALRM ) == -1 )
		set_in_error();
	if ( sigprocmask( SIG_BLOCK, &sigs, &prev_sigs ) == -1 )
		set_in_error();
	
	
	//Set the head/tail of the paddle based on direction
	int *head, *tail;
	if ( dir == MOVE_UP ){
		head = &_paddle.top;
		tail = &_paddle.bottom;
	}else{
		head = &_paddle.bottom;
		tail = &_paddle.top;
	}


	//Draw the paddle in the new location
	if ( start_draw( ) == DRAW_OK ) {
		mvaddch(*tail, _paddle.col, ' ');		
		mvaddch(*head + (dir), _paddle.col, _paddle.pad_char);
		move(LINES - 1, COLS - 1);
		refresh();
		end_draw( );
	}
	
	
	//Update the coordinates of the paddle
	_paddle.bottom += dir;
	_paddle.top += dir;
	
	//At this point, ask the referee to check for paddle contact.
	//I ask the referee so that the paddle doesn't need to know
	//anything about the ball state. referee_check_paddle() basically
	//passes through to ball_bounce_or_lose() for now, but could do
	//more in the future.
	referee_check_paddle();
	
	//Go back to previous set of sig masks
	if ( sigprocmask( SIG_SETMASK, &prev_sigs, NULL ) == -1 )
		set_in_error();
	
}

	
