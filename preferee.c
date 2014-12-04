#include	<stdio.h>
#include	<curses.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	"pball.h"
#include	"pcourt.h"
#include	"ponglib.h"
#include	"ppaddle.h"
#include	"preferee.h"
#include	"pjmbtrn.h"




//Keep a reference to the court geometry
static ppcourt_geom court_g = {0};

//Keep a reference to the ball count. Decrement upon "lose"
static int ball_count = STARTING_BALL_COUNT;

//Keep x and y locations for the "fin" messsage
static int x_fin, y_fin;
static const char * FIN_MSG = "FIN.";
static const char * PLAY_AGAIN_MSG = "Play Again? Type 'r'";
static const char * DONE_MSG = "Done? Type 'Q'.";
static const char * BLANK_MSG= "                           ";

//Static function prototypes
//static void referee_end_game();



/*referee_init() - Initialize the referee object
*
*/
void referee_init()
{
	//Get a reference to the court gerometry
	court_geom( &court_g );
	
	x_fin = court_g.left + ((court_g.right - court_g.left) / 2);
	y_fin = court_g.top + ((court_g.bottom - court_g.top) / 2);


}


/*referee_check_ball() - check the ball and report on its current position.
*	Arguments: int, int - the y and x positions of the ball to check 
*   Returns: int, a mask containing the ball location status
*
*/
int referee_check_ball( int y, int x )
{
	
	int pos_mask = 0;
	
	//Check for paddle contact
	pad_c pad_contatct_state = paddle_contact( y, x );
	if ( pad_contatct_state == PAD_CONTACT )
	{
		pos_mask |= BALL_PADDLE;
	} 
	else if ( pad_contatct_state == PAD_INTERCEPT )
	{
		pos_mask |= BALL_PADDLE;
		pos_mask |= BALL_PADDLE_INTERCEPT;
	}

	//Check for top/bottom wall
	if (  y == court_g.top + 1 )
		pos_mask |= BALL_TOP_WALL;
	else if (  y == court_g.bottom - 1 )
		pos_mask |= BALL_BOT_WALL;

	//Check for left wall or lose
	if ( x == court_g.left + 1 ){		/*left wall*/
		pos_mask |= BALL_LEFT_WALL;
	
	}else if ( x >= court_g.right  ) {	/*lose*/
		if (ball_count == 0){
			pos_mask |= NO_BALLS;
		}else{
			ball_count--;
			jmbtrn_display_ball_count(ball_count);
		}
		pos_mask |= BALL_OB;
	}

	return pos_mask;
	
}



/*referee_check_paddle() - double check the whether the paddle is
*   touching or intercepting the ball. For now, this is basically a 
*   pass through to ball_bounce_or_lose(), largely to isolate the paddle
*   from needing to know anything about the ball. Also, in some future state
*   if the referee is 'managing' multiple balls, it could handle checking
*   all of them.
*    
*/
void referee_check_paddle( ){

	ball_bounce_or_lose( );

}




/*referee_end_game( )
*	method to end the game when no balls are left. Displays a
*	message in the center of the court and allows user to "play again" with 
*	the 'r' key, or quit with 'Q'
*
*/
void referee_end_game()
{

	//Stop the ticker
	set_ticker(0);
	
	//Display an end game message
	mvaddstr(y_fin, x_fin, BLANK_MSG);
	mvaddstr(y_fin + 1, x_fin, BLANK_MSG);
	mvaddstr(y_fin + 2, x_fin, BLANK_MSG);
	mvaddstr(y_fin, x_fin, FIN_MSG);
	mvaddstr(y_fin + 1, x_fin, PLAY_AGAIN_MSG);
	mvaddstr(y_fin + 2, x_fin, DONE_MSG);
	
	refresh();
	
}



/*referee_restart_game( )
*	function to 'reload' the ball sleeve, and set the ticker a-runnin.
*	Don't need to protect drawing here, cause we'll stop the ticker and
*	just brute force the cleanup. 
*
*/
void referee_restart_game()
{
	
	//Stop the ticker, just in case
	set_ticker(0);
	
	//Blank out the fin message area
	mvaddstr(y_fin, x_fin, BLANK_MSG);
	mvaddstr(y_fin + 1, x_fin, BLANK_MSG);
	mvaddstr(y_fin + 2, x_fin, BLANK_MSG);
	refresh();

	//Reset the ball count
	ball_count = STARTING_BALL_COUNT;
	jmbtrn_display_ball_count(ball_count);
	
	//Reset the timer
	jmbtrn_zero_clock_display();
	
	//re-serve the ball
	ball_serve();
	
	//start the ticker
	if ( set_ticker( 1000 / (TICK_PER_SEC) ) == -1 ){
		set_in_error();
	}
	
}


