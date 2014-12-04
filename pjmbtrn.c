/*
* pjmbtrn.c - implementation file for Jumbotron object.
			The jumbotron is responsible for displaying the ball count
			and the elapsed time.
*/


#include	<stdio.h>
#include	<curses.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<string.h>
#include	"pjmbtrn.h"
#include	"ponglib.h"

#define		LABEL_GAP 20	/*The space between the ball count and timer*/


typedef struct {
	int x_pos;			/*The starting x position of the jumbotron*/
	int y_pos;			/*The y position of the jumbotron*/
	int ball_cnt_x;		/*The x position of the ball count VALUE*/
	int ball_cnt_y;		/*The y position of the ball count VALUE*/
	int time_disp_x;	/*The x position of the elapsed time VALUE*/
	int time_disp_y;	/*The y position of the elapsed time VALUE*/
	unsigned long clock_value;	/*Store total # of ticks*/
	int clock_delay;	/*#ticks until the timer updates*/
} ppjmbtrn;



static const char BALLS_LEFT_LABEL[] = "BALLS LEFT: ";	/*Balls left label*/
static const char TIME_LABEL[] = "TOTAL TIME: ";		/*Total time label*/
static const int CLOCK_DELAY = TICK_PER_SEC;			/*Update every sec*/
static ppjmbtrn _jt;						/*Static singleton jumbotron*/

//static function prototypes.
static void jmbtrn_display_time( int );



/*jmbtrn_init - sets the initial properties*/
void jmbtrn_init( int y, int x )
{
	
	_jt.x_pos = x;
	_jt.y_pos = y;
	_jt.ball_cnt_x = _jt.x_pos + (int)strlen(BALLS_LEFT_LABEL);
	_jt.ball_cnt_y = y;
	_jt.time_disp_x = _jt.ball_cnt_x + LABEL_GAP + (int)strlen(TIME_LABEL);
	_jt.time_disp_y = y;
	_jt.clock_delay = CLOCK_DELAY;
	
	
	//Add the 'balls left' label
	mvaddstr( _jt.ball_cnt_y, _jt.x_pos, BALLS_LEFT_LABEL );
	
	//Add the 'Total time' label
	mvaddstr( _jt.time_disp_y, _jt.ball_cnt_x + LABEL_GAP, TIME_LABEL );
	
	//Refresh	
	refresh();
	
	//Set the time display to 0
	jmbtrn_zero_clock_display();
	//jmbtrn_display_time( 0 );


}


/*jmbtrn_zero_clock_display() - Reset the clock to zero
*	
*/
void jmbtrn_zero_clock_display( )
{
	_jt.clock_value = 0;
	jmbtrn_display_time( 0 );
}



/*jmbtrn_process_clock_tick() - Initialize and draw the court
*	
*	Called from the sigaction handler for the SIGALRM. Increments the 
*	clock value by 1 tick and decides if another second has elapsed and 
*	needs to be updated.
*/
void jmbtrn_process_clock_tick( )
{
	_jt.clock_value++;
	if ( _jt.clock_value % CLOCK_DELAY == 0 ){
		jmbtrn_display_time( _jt.clock_value / CLOCK_DELAY );
	}
}



/*jmbtrn_display_time() - Update the time display
*	
*	Args: int - time in seconds
*	
*	Formats the time and the displays it using curses.
*/
static void jmbtrn_display_time( int time_secs )
{

	//Get the current position of the cursor
	//int y, x;
	//getyx(stdscr, y, x);
	
	//Format and display the time
	char buf[5];
	memset( &buf, '\0', 2);
	sprintf( buf, "%i:%02i", time_secs / 60, time_secs % 60  );
	
	//Draw the updated time
	if ( start_draw( ) == DRAW_OK ) {
		mvaddstr( _jt.time_disp_y, _jt.time_disp_x, buf );
		move(LINES-1, COLS-1);		/* park cursor	*/
		refresh();
		end_draw( );
	}

}


/*jmbtrn_display_ball_count() - Initialize and draw the court
*	Arguments:
*		int - the number of balls to display on the jumbotron
*
*	Formats display for the ball count, and updates the jumbotron
*	after first getting a draw lock using start_draw().
*
*/
void jmbtrn_display_ball_count( int ball_count )
{
		
	//Update the ball count
	int buf_length = 3;
	char buf[buf_length];
	memset( &buf, '\0', buf_length);
	sprintf( buf, "%02i", ball_count );
	
	
	if ( start_draw( ) == DRAW_OK ) {
		mvaddstr( _jt.ball_cnt_y, _jt.ball_cnt_x, buf );
		move(LINES-1, COLS-1);		/* park cursor	*/
		refresh();
		end_draw( );
	}

}
