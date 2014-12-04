/*
* pjmbtrn.h - header file for public interface of pong Jumbotron object.
			The jumbotron is responsible for displaying BOTH the ball count
			and the elapsed time.
*/


/*jmbtrn_init() - Initialize and draw the court
*	Arguments:
*		int - the y coordinate of the jumbotron display
*		int - the x coordinate of the first char in the jumbotron
*/
void jmbtrn_init( int, int );


/*jmbtrn_zero_clock_display() - Reset the clock to zero
*	
*/
void jmbtrn_zero_clock_display( );


/*jmbtrn_process_clock_tick() - Initialize and draw the court
*	Called from the sigaction handler for the SIGALRM. Increments the 
*	clock value by 1 tick and decides if another second has elapsed and 
*	needs to be updated.
*/
void jmbtrn_process_clock_tick( );


/*jmbtrn_display_ball_count() - Initialize and draw the court
*	Arguments:
*		int - the number of balls to display on the jumbotron
*
*/
void jmbtrn_display_ball_count( int );
