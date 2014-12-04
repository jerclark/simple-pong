#include	<sys/time.h>
#include	<signal.h>
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"pball.h"
#include	"pjmbtrn.h"
#include    "ponglib.h"



//Keep the current "draw" toggle
static draw_stat_t d_state = DRAW_OK;


//Variable to store whether the game should "end"
//normally due to an error
static int _inerror = 0;


/*random_number( int ) - generate a random number
*	Arguments:
*		int - the maximum number to be returned. This is used
*			primarily for generating a random ball speed, but 
*			could be used by anything needing a random number.
*/
int random_number( int max ){
	int r = 1;
	r = (rand() % max);
	return r;
}



/*STRAIGHT LIFT FROM ALARMLIB.C*/
int set_ticker( int n_msecs )
/*
 *	arg in milliseconds, converted into micro seoncds
 *	Returns -1 on error, 0 if no error
 */
{
	struct itimerval new_timeset, old_timeset;
	long	n_sec, n_usecs;

	n_sec = n_msecs / 1000 ;
	n_usecs = ( n_msecs % 1000 ) * 1000L ;

	new_timeset.it_interval.tv_sec  = n_sec;	/* set reload  */
	new_timeset.it_interval.tv_usec = n_usecs;	/* new ticker value */
	new_timeset.it_value.tv_sec     = n_sec  ;	/* store this	*/
	new_timeset.it_value.tv_usec    = n_usecs ;	/* and this 	*/

	if ( setitimer( ITIMER_REAL, &new_timeset, &old_timeset ) != 0 ){
		printf("Error with timer..errno=%d\n", errno );
		return -1;
	}
	return 0;
}


/*handle_tick( int s ) - handles the SIGALRM ticks
* Arguments:
*	int - the calling signal ID
*/
void handle_tick( int s )
{

	jmbtrn_process_clock_tick( );
	ball_move( );
	
}





/*start_draw() - ask for permission to draw
*   We do this to prevent colliding while drawing
*   in curses. Before drawing, calling functions must 
*   get permission from the ponglib.
*
*  returns: draw_stat_t, either DRAW_OK or DRAW_LOCKED
*
*/
draw_stat_t start_draw( ){
	
	draw_stat_t ret = d_state;
	
	if ( d_state == DRAW_OK )
		d_state = DRAW_LOCKED;
		
	return ret;
	
};



/*end_draw() - tell the shared lib that I'm
*    done drawing.
*
*  returns: draw_stat_t, either DRAW_OK or DRAW_LOCKED
*
*/
draw_stat_t end_draw( ){
	
	draw_stat_t ret;
	
	ret = d_state = DRAW_OK;
		
	return ret;
	
}


/*set_in_error() - sets _inerror to true
*
*/
void set_in_error(){
	_inerror = 1;
}



/*in_error() - returns the _inerror variable
*
*/
int in_error(){
	return _inerror;
}
