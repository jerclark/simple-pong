/*
* ppcourt.h - header file for public interface of pong court
*/


/*ppcourt_geom
*	Data structure to represent the court's geometry
*/
typedef struct {
	int top,
		bottom,
		left,
		right;
} ppcourt_geom;


/*court_init() - Initialize and draw the court
*	Arguments:
*		int - the buffer to use for the court edges. For example, 
*			passing in 3 will draw the court edges 3 rows/columns from the
*			edge of the screen.
*/
void court_init(int);



/*court_geom() - get the current court geometry
*	Arguments:
*		ppcourt_geom * - a pointer to a ppcourt_geom struct that will be
*			filled by the court object's current state
*	Will set ppcourt_geom to all 0 if the court isn't initialized
*/

void court_geom(ppcourt_geom * );
