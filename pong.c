#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	"pcourt.h"
#include	"pball.h"
#include	"ponglib.h"
#include	"ppaddle.h"
#include	"pjmbtrn.h"
#include	"preferee.h"


/*
 *	pong.c	
 *
 *	Main file for pong
 */


//Function prototypes
void set_up();
void wrap_up();


/** the main loop **/

int main()
{
	int	c;

	set_up();

	while ( ( c = getch() ) != 'Q' && !in_error() ){  //in_error will
		//Respond to up/down keystrokes here		   //be set by failing
		if ( c == 'k' )								   //syscalls
            paddle_up();
        else if ( c == 'm' )
            paddle_down();
        else if ( c == 'r' )
        	referee_restart_game();
	}
	wrap_up();
	
	//If in error, then exit 1
	if ( in_error() )
		exit(1);
	
	return 0;
}




void set_up()
{

	initscr();		   /* turn on curses	*/
	noecho();		   /* turn off echo	*/
	cbreak();		   /* turn off buffering	*/
	srand( getpid() ); /*seed the random num generator*/

	signal(SIGINT, SIG_IGN);	/* ignore SIGINT	*/
	signal(SIGTERM, SIG_IGN);	/* ignore SIGINT	*/
	
	//Draw the court
	court_init( 3 );
	
	//Init the scoreboard display
	ppcourt_geom court_g;
	court_geom( &court_g );
	jmbtrn_init( court_g.top - 1, court_g.left + 1 ); /*Init's clock to 0*/
	jmbtrn_display_ball_count( STARTING_BALL_COUNT );
	
	//Init the referee
	referee_init();
	
	//Init the paddle
	paddle_init( );
	
	//Init and serve the ball
	ball_init( );
	
	//Create a sigaction with "handle_tick" as the handler
	struct sigaction tick_handler;
	tick_handler.sa_handler = handle_tick;
	tick_handler.sa_flags |= SA_RESTART; 	/*Make sure to restart sys calls*/
	if ( sigaction( SIGALRM, &tick_handler, NULL ) == -1 ){
		perror( "Sigaction" );
		exit( 1 );
	}else{
		if ( set_ticker( 1000 / (TICK_PER_SEC) ) == -1 ){/* send millisecs per tick */
			perror( "Error setting ticker" );
			exit( 1 );
		}	
	}
	
	
}




/* stop ticker and curses */
void wrap_up()
{
	set_ticker( 0 );
	endwin();		/* put back to normal	*/
}




