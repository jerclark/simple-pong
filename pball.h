/*
* pball.h - header file for public interface of pong Ball object
*/


/*ball_init() - Initialize the ball object
*
*/
void ball_init();



/*ball_move() - move the ball. Signal handler for SIGALRM
*
*/
void ball_move();


/*ball_serve() - serve the ball from the center of the court
*
*/
void ball_serve();



/* ball_bounce_or_lose: if ball hits walls, change its direction
 *   rets: int - 0 (LOSE), 1 (BOUNCE)
 */
int ball_bounce_or_lose();



