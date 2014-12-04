/*
* ponglib.h - header file for pong utility functions
*/

#define	TICK_PER_SEC 75 /* affects speed */
#define	STARTING_BALL_COUNT 2 /* affects speed */
#define CONCURRENT_BALL_COUNT 1 /*Allows for multi-ball*/


//Global flag to set whether the game should be 'over' 
//due to a system call failure


/* draw_stat_t
*	enum to store current stat of the "draw" lock
*   this is used to prevent drawing race conditions.
*/
typedef enum {
	DRAW_OK,
	DRAW_LOCKED
} draw_stat_t;





/*random_number( int ) - Initialize the ball object
*	Arguments:
*		int - the maximum factor to be returned. This is used
*			primarily for generating a random ball speed, but 
*			could be used by anything needing a random number.
*/
int random_number( int );



/*set_ticker( int ) - Set the timer
*	Arguments:
*		int - the maximum factor to be returned. This is used
*			primarily for generating a random ball speed, but 
*			could be used by anything needing a random number.
*/
int set_ticker( int );



/*handle_tick( ) - Handle the timer ticks from SIGALRM
*
*/
void handle_tick( int );



/*start_draw() - ask for permission to draw
*   We do this to prevent colliding while drawing
*   in curses. Before drawing, calling functions must 
*   get permission from the ponglib.
*
*  returns: draw_stat, either DRAW_OK or DRAW_LOCKED
*
*/
draw_stat_t start_draw( );



/*end_draw() - tell the shared lib that I'm
*    done drawing.
*
*  returns: draw_stat, either DRAW_OK or DRAW_LOCKED
*
*/
draw_stat_t end_draw( );


/*set_in_error() - sets global error state to true
*
*/
void set_in_error();


/*in_error() - returns the in_error global variable
*
*/
int in_error();




