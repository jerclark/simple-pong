//Bitset used to populate the bitmask
//that's returned from referee_check_ball
#define BALL_TOP_WALL (1 << 0)
#define BALL_LEFT_WALL (1 << 1)
#define BALL_BOT_WALL (1 << 2)
#define BALL_OB (1 << 3)
#define BALL_PADDLE (1 << 4)
#define BALL_PADDLE_INTERCEPT (1 << 5)
#define NO_BALLS (1 << 6)	

/*
* preferee.h - header file for public interface of pong Referee object
*/



/*referee_init() - Initialize the referee object
*
*/
void referee_init();




/*referee_ball_stat() - check the ball and report on
*	Arguments: int, int - the y and x positions of the ball to check 
*   Returns: int, a mask containing the ball location status
*
*/
int referee_check_ball( int, int);



/*referee_handle_paddle_move() - double check the whether the paddle is
*   touching or intercepting the ball.
*    
*   Returns: int, a mask containing the ball location status
*
*/
void referee_check_paddle( );



/*referee_end_game( )
*	function to end the game when no balls are left. Displays a
*	message in the center of the court and allows user to "play again"
*
*/
void referee_end_game( );



/*referee_restart_game( )
*	function to 'reload' the ball sleeve, and set the ticker a-runnin.
*
*/
void referee_restart_game();


