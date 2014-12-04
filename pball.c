#include	<stdio.h>
#include	<curses.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	"pball.h"
#include	"pcourt.h"
#include	"ponglib.h"
#include	"preferee.h"


#define	SYMBOL	'O'
#define BLANK	' '

//Values returned from ball_bounce_or_lose
#define LOSE 0
#define BOUNCE 1
#define GAME_OVER 2

/* ppball - Data structure to maintain ball state
* x_delay - controls the horizontal speed
* x_pos - current h position of the ball
* x_dir - current h direction of the ball (-1, left; 1, right)
* x_count - ticks up until next move
* x_srv - the horizontal serve location
* y_delay - controls the vertical speed
* y_pos - current v position of the ball
* y_dir - current v direction of the ball (-1, up; 1, down)
* y_count - ticks up until next move
* y_srv - the vertical serve location
* symbol - the ball symbol
*
*/
typedef struct {
	int x_delay, x_pos, x_dir, x_count,
		y_delay, y_pos, y_dir, y_count,
		y_srv, x_srv;
	chtype symbol;
} ppball;


//Static variable to store ball data
static ppball _ball;


/*
Variables to control the ball speed. They will be set during
the ball_init() method based on the size of the court.
*/	
static int _min_delay, _max_delay, _delay_range = 5;

//Static variable to hold court geometry
static ppcourt_geom court_g = {0};


//Static function declarations
static void ball_blank();
static void ball_draw();




/*ball_init() - Initialize the ball object
*
*	Populates the static ball struct with the initial data.
*	Gets a pointer to the court geometry, which I'm not crazy about
*	but I thought it would be OK for the purposes of this project. It 
*   needs the court geometry to figure out the x and y coordinates for
*	serving (by default the center of the court), which are stored as part
*	of the ball state.
*   This method also sets the "speed range" by calculating a delay range
*	based on the size of the court.
*
*	Once the ball is init'd this function will automatically serve the ball.
*   
*/
void ball_init(){

	//Get the boundries of the court, then
	//set the starting pos and the delay ranges
	court_geom( &court_g );
	if ( &court_g != (ppcourt_geom *)NULL) {
		int court_height = (court_g.bottom - court_g.top);
		int court_width = (court_g.right - court_g.left);
		
		_ball.y_srv = _ball.y_pos = court_g.top + (court_height / 2);
		_ball.x_srv = _ball.x_pos = court_g.left + (court_width / 2);
		
		//Calculate the delay based on court width.
		//The delay should be smaller for faster play, 
		//so as the court gets bigger the ball will go
		//faster. 320 and 640 are based somewhat on a 
		//standard width of around 80
		_min_delay = 320/court_width;
		_max_delay = 640/court_width;
		_delay_range = _max_delay - _min_delay;	
		
	}
	
	
	//Set the rest of the values
	_ball.y_count = _ball.y_delay = 10;
	_ball.x_count = _ball.x_delay = 10;
	_ball.y_dir = 1  ;
	_ball.x_dir = -1  ;
	_ball.symbol = (chtype)SYMBOL;
	
	//Serve the ball
	ball_serve();
	
}



/*ball_serve() - Initialize the ball object
*
*	Function to serve the ball. Gets called on init and when 
*	the ball goes out of bounds as detected in ball_move().
*
*	Serves the ball with a pseudo-random speed and direction, from the x_srv
*	and y_srv coordinates. The ball is served away from the paddle.
*   
*/
void ball_serve()
{

	ball_blank();
	
	//Set the starting location
	_ball.y_pos = _ball.y_srv;
	_ball.x_pos = _ball.x_srv;
	
	//Set the rest of the values
	_ball.y_count = _ball.y_delay = 
		random_number( _delay_range ) + _min_delay;
	_ball.x_count = _ball.x_delay = 
		random_number( _delay_range ) + _min_delay;
	_ball.y_dir = 1  ;
	_ball.x_dir = -1  ;
	
	
	ball_draw();
	
}




/*ball_move() - move the ball
*	
*	Determines if the x and/or y delays have been exhausted, then checks
*	to see if it should bounce, lose, or just move normally. If the ball
*	goes out of bounds (lose), and there are still some balls left 
*	it will automatically re-serve(). If the ball should bounce, the 
*	ball state is updated by ball_bounce_or_lose, and ball_draw() will be
*	called to draw the ball in its next spot.
*   
*/
void ball_move()
{
	int	y_cur, x_cur, moved;

	
	y_cur = _ball.y_pos ;		/* old spot		*/
	x_cur = _ball.x_pos ;
	moved = 0 ;

	if ( _ball.y_delay > 0 && --_ball.y_count == 0 ){
		_ball.y_pos += _ball.y_dir ;	/* move	*/
		_ball.y_count = _ball.y_delay  ;	/* reset*/
		moved = 1;
	}

	if ( _ball.x_delay > 0 && --_ball.x_count == 0 ){
		_ball.x_pos += _ball.x_dir ;	/* move	*/
		_ball.x_count = _ball.x_delay  ;	/* reset*/
		moved = 1;
	}

	if ( moved ){ 	
		mvaddch(y_cur, x_cur, BLANK);  /*blank it out*/
		int stat = ball_bounce_or_lose();
		if (stat == GAME_OVER){
			
			referee_end_game();
		
		}else if (stat == LOSE ){
			
			ball_serve();             /*re-serve*/
			
		}else{
			
			ball_draw();	
			
		}
	}

}




/* ball_bounce_or_lose: if ball hits walls, change its direction
 *   rets: int - LOSE(0) or BOUNCE(1)
 *  
 *	Calls out to referee_check_ball which is responsible for determining
 *	where the ball is relative to the walls/paddle/OB. referee_check_ball
 *	returns a mask indicating 'where' the ball currently is, and this function
 *	will adjust the ball state accordingly. 
 *
 *	If the BALL_PADDLE_INTERCEPT bit is set by referee_check_ball, that means 
 *	the paddle moved to a cell where the ball currently sits. In this case, 
 *  the ball state is adjusted left by 1 cell so that the ball is put "back
 *	in play"
 */
int ball_bounce_or_lose()
{
	
	int ball_pos_mask = referee_check_ball( _ball.y_pos, _ball.x_pos );
	
	if ( ball_pos_mask & NO_BALLS )      //Last ball OB
		return GAME_OVER;
		
	if ( ball_pos_mask & BALL_OB )	 	 //Ball OB
		return LOSE;
	
	if ( ball_pos_mask & BALL_TOP_WALL ) //Bounce down
		_ball.y_dir = 1;
	if ( ball_pos_mask & BALL_BOT_WALL ) //Bounce up
		_ball.y_dir = -1;
	if ( ball_pos_mask & BALL_LEFT_WALL ) //Bounce right
		_ball.x_dir = 1;
	if ( ball_pos_mask & BALL_PADDLE ){   //Bounce left and adjust speed
		_ball.x_dir = -1;
		_ball.y_count = _ball.y_delay = 
			(random_number( _delay_range ) + _min_delay);
		_ball.x_count = _ball.x_delay = 
			(random_number( _delay_range ) + _min_delay);	
	}
	if ( ball_pos_mask & BALL_PADDLE_INTERCEPT ) //Put ball back in play
		_ball.x_pos -= 1;
	
	return BOUNCE;
	
}


/* ball_draw: draw the ball in its current position
 *  
 *	Basically a wrapper around the curses drawing routines. Uses the 
 *	start_draw() locking mechanism provided by ponglib.
 */
static void ball_draw(){

	if ( start_draw( ) == DRAW_OK ){
		mvaddch(_ball.y_pos, _ball.x_pos, _ball.symbol);
		move(LINES-1, COLS-1);		/* park cursor	*/
		refresh();
		end_draw();
	}
	
}



/* ball_blank: draw a blank in the ball's current position
 *  
 */
static void ball_blank(){

	mvaddch(_ball.y_pos, _ball.x_pos, BLANK);
	move(LINES-1, COLS-1);		/* park cursor	*/
	refresh();
	end_draw();
	
}





