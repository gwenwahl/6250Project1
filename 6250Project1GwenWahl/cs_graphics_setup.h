#pragma once
/*
*** FILE NAME   : cs_graphics_setup.h
*** DESCRIPTION : This is a header file to be used in CS4250/5250/6250 assignments.
It contains initialization function calls and setups for
OpenGL programs with GLUT.  The initializations involve
a callback hander definition which sets viewing parameters.
*** DATE        : Jan. 2009
*** WRITTEN By  : JKL
*/

//@@***********************************************************************************@@
void reshape_handler(int width, int height)
{

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();             
	
	glOrtho(0, width, 0, height, -100, 1000);
}	// end of reshape_handler()

	//@@***********************************************************************************@@
void init_setup(int width, int height, char *windowName)
{
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // single buffer, rgb color
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // double buffer, rgb color
	glutInitWindowSize(width, height);			  // init. window size
	glutInitWindowPosition(5, 5);				  // init. window position
	glutCreateWindow(windowName);				  // window name
	glutReshapeFunc(reshape_handler);		      // sets the reshape call back
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white and opaque
	
}	// end of init_setup()
