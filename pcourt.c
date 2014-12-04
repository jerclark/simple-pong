#include	<stdio.h>
#include	<curses.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	"pcourt.h"

typedef struct {
	int top;		/*The top edge of the court*/
	int bottom;		/*The bot edge of the court*/
	int left;		/*The left edge of the court*/
	int right;		/*The right edge of the court*/
	chtype h_char;	/*The char for top and bot edge*/
	chtype v_char;	/*The char for the left edge*/
} ppcourt;


//Static variable to store the "singleton" court object
static ppcourt _court = {0};


/*init() - Initialize and draw the court
*	Arguments:
*		int - the buffer to use for the court edges. For example, 
*			passing in 3 will draw the court edges 3 rows/columns from the
*			edge of the screen.
*
*	Gets the current screen size using ncurses getmaxyx.
*	Performs basic layout logic and math to determine where the walls should be
*	drawn. Then the lines are drawn.
*/
void court_init(int gutter_size){

	//Get the current screen size
	int y, x;
	getmaxyx( stdscr, y, x);
	
	//Make sure window is big enough
	if ( y < (gutter_size * 4) )
		perror("Window too small to play");
	
	//set values in ppcourt struct
	_court.top = gutter_size;
	_court.bottom = y - (gutter_size + 1);
	_court.left = gutter_size;
	_court.right = x - (gutter_size + 1);
	_court.h_char = (chtype)'-';
	_court.v_char = (chtype)'|';
	
	//Draw the lines
	mvhline(_court.top, _court.left, _court.h_char, _court.right - _court.left); //top line
	mvhline(_court.bottom, _court.left, _court.h_char, _court.right - _court.left); //bottom line
	mvvline(_court.top + 1, _court.left, _court.v_char, _court.bottom - _court.top - 1); //left line
	
	//Draw the instructions
	mvaddstr( _court.bottom + 2, _court.left + 2, "k=up, m=down, Q=quit" );
		

}


/*court_geom() - get the current court geometry
*	Arguments:
*		ppcourt_geom * - a pointer to a ppcourt_geom struct that will be
*			filled by the court object's current state
*	Will set ppcourt_geom to all 0 if the court isn't initialized
*/
void court_geom(ppcourt_geom * geom){
	
	//If we're not initalized, set ppcourt_geom to NULL
	if ( _court.top == 0 ){
		geom = (ppcourt_geom *)NULL;
	}else{
		geom->top = _court.top;
		geom->bottom = _court.bottom;
		geom->left = _court.left;
		geom->right = _court.right;
	}

}

