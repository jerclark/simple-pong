/*
* ppaddle.h - header file for public interface of pong Paddle object
*/


typedef enum {
	PAD_NO_CONTACT,
	PAD_CONTACT,
	PAD_INTERCEPT
} pad_c;


/*paddle_init() - Initialize the paddle object
*
*/
void paddle_init();



/*paddle_up() - move the paddle up one row.
*/
void paddle_up( );



/*paddle_down() - move the paddle up one row.
*/
void paddle_down( );



/*paddle_contact() - test if ball is contact with paddle.
* Arguments:
*	int - y coord of the ball to check
*   int - x coord of the ball to check
*/
pad_c paddle_contact( int, int );

