/*
*** FILE NAME   : animation_example.c
*** DESCRIPTION : This program is an example program using OpenGL for animation.
*** DATE        : May 2012
*** WRITTEN By  : JKL
*/

#include "stdafx.h"
#include <stdio.h>               // standard C libraries
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "cube.h"
#include <GL/glut.h>             // GLUT library
#include "cs_graphics_setup.h"   // Header for CS4250/5250/6250 courses
using namespace std;

//@@***********************************************************************************@@
// Constants
#define WINDOW_XS 520							// Window size
#define WINDOW_YS 520
#define WINDOW_NAME "Gwen Wahl 6250 Project 1"	// Window name

#define ANI_MSEC 10	 // gap between frames

//@@***********************************************************************************@@
// Function prototypes
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void animation_func(int val);
cube loadCube(std::string fileName);
vector<string> tokenizeString(string s);

//Global Cube
cube myCube = loadCube("data2.dat");
bool perspective = false;

//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);
	cout << "Press x or X to move the object along the x axis" << endl;
	cout << "Press y or Y to move the object along the y axis" << endl;
	cout << "BONUS: Press z or Z to move the object along the z axis" << endl;
	cout << "Press r to rotate the object 15 degrees around the y axis" << endl;
	cout << "Press p to switch between parallel and perspective projection" << endl;
	cout << "Press D to reset the object to it's default state" << endl;
	cout << "Press q or Q to quit." << endl;

	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutTimerFunc(ANI_MSEC, animation_func, 0);
	glutMainLoop();

	return 1;

}	// end of main()

	//@@***********************************************************************************@@
void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	//Switch to perspective
	if (perspective) {
		glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
		glLoadIdentity();             // Reset
									  // Enable perspective projection with fovy, aspect, zNear and zFar
		gluPerspective(120.0f, 1, 0, 1000000.0f);
	}
	else {
		glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
		glLoadIdentity();             // Reset
		glOrtho(0, 520, 0, 520, -100, 1000);
	}

	//Display Cube
	myCube.display();

	glFlush();
	glutSwapBuffers();	// double buffering

}	// end of display_func()

	//@@***********************************************************************************@@
void animation_func(int val)
{
	glutPostRedisplay();
	glutTimerFunc(ANI_MSEC, animation_func, 0);

}//end animation_func

 //@@***********************************************************************************@@
void keyboard_func(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'Q':
	case 'q':
		printf("Good Bye !\n");
		exit(0);				 // terminates the program
		break;
	case 'x':
		myCube.adjustCurrentTranslationX(-10);
		break;
	case 'X':
		myCube.adjustCurrentTranslationX(10);
		break;
	case 'y':
		myCube.adjustCurrentTranslationY(-10);
		break;
	case 'Y':
		myCube.adjustCurrentTranslationY(10);
		break;
	case 'r':
		myCube.adjustCurrentRotation(15);
		break;
	case 'D':
		myCube.reset();
		if (perspective) {
			myCube.adjustCurrentTranslationZ(-150);
			myCube.adjustCurrentTranslationX(-260);
			myCube.adjustCurrentTranslationY(-260);
		}
		break;
	case 'z':
		myCube.adjustCurrentTranslationZ(-5);
		break;
	case 'Z': 
		myCube.adjustCurrentTranslationZ(5);
		break;
	case 'p':
		if (perspective) {
			perspective = false;
			myCube.reset();
		}
		else {
			perspective = true;
			myCube.reset();
			//Translation adjusts coordinate system
			myCube.adjustCurrentTranslationZ(-150);
			myCube.adjustCurrentTranslationX(-260);
			myCube.adjustCurrentTranslationY(-260);
		}
		break;
	}  // end of switch

}	// end of keyboard_func()

cube loadCube(std::string filename) {
	std::string line;
	std::ifstream dataFile(filename);
	std::vector<std::string> lines;
	if (dataFile.is_open()) {
		while (std::getline(dataFile, line)) {
			lines.push_back(line);
		}
		dataFile.close();
	}
	else {
		std::cout << "Unable to open file";
	}

	//First line defines number of indices and connections
	vector<string> l = tokenizeString(lines[0]);
	int numIndices = stoi(l[0]);
	int numConnections = stoi(l[1]);
	vector<pt> indices;
	vector<lineSegment> lineSegments;

	//Get indices
	for (int i = 2; i < numIndices + 2; ++i) {
		vector<string> l = tokenizeString(lines[i]);
		pt point;
		point.x = stoi(l[0]);
		point.y = stoi(l[1]);
		point.z = stoi(l[2]);
		indices.push_back(point);
	};
	//Get connecting line segments
	for (int i = 2 + numIndices; i < lines.size(); ++i) {
		vector<string> l = tokenizeString(lines[i]);
		lineSegment s;
		s.pointOne = indices[stoi(l[0])];
		s.pointTwo = indices[stoi(l[1])];
		lineSegments.push_back(s);
	}
	cube c(indices, lineSegments);
	return c;
};

vector<string> tokenizeString(string s) {
	//Strip leading whitespace
	size_t startpos = s.find_first_not_of(" \t");
	if (string::npos != startpos)
	{
		s = s.substr(startpos);
	}

	regex reg("\\s+");
	sregex_token_iterator iter(s.begin(), s.end(), reg, -1);
	sregex_token_iterator end;
	vector<string> vec(iter, end);
	return vec;
}

cube::cube(vector<pt> ind, vector<lineSegment> con) {
	indices = ind;
	lines = con;
	//Translate to center of screen
	currentTranslationX = 260;
	currentTranslationY = 260;
	currentTranslationZ = 0;
	//Calculate the centroid
	double totalX = 0;
	double totalY = 0;
	double totalZ = 0;
	for (auto a : indices) {
		totalX += a.x;
		totalY += a.y;
		totalZ += a.z;
	}
	centroid.x = round(totalX / indices.size());
	centroid.y = round(totalY / indices.size());
	centroid.z = round(totalZ / indices.size());
}

cube::~cube()
{
}

void cube::adjustCurrentTranslationX(int xOffset) {
	currentTranslationX += xOffset;
}
void cube::adjustCurrentTranslationY(int yOffset) {
	currentTranslationY += yOffset;
}
void cube::adjustCurrentRotation(int rOffset) {
	currentRotation += rOffset;
	if (currentRotation >= 360) {
		currentRotation = currentRotation - 360;
	}
	else if (currentRotation < 0) {
		currentRotation = currentRotation + 360;
	}
}
void cube::adjustCurrentTranslationZ(int zOffset) {
	currentTranslationZ += zOffset;
}
void cube::reset() {
	currentRotation = 0;
	currentTranslationX = 260;
	currentTranslationY = 260;
	currentTranslationZ = 0;
}
void cube::display() {
	glColor3f(1, 0, 0);
	//glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//These are in reverse order
	//Translate to final position
	glTranslatef(getCurrentTranslationX(), getCurrentTranslationY(), getCurrentTranslationZ());
	//Translate away from center with respect to centroid
	glTranslatef(centroid.x, centroid.y, centroid.z);
	//Rotate
	glRotatef(currentRotation, 0, 1, 0);
	//Translate to origin with respect to centroid
	glTranslatef(-1 * centroid.x, -1 * centroid.y, -1 * centroid.z);
	//Draw the lines
	glBegin(GL_LINES);
	for (auto a : lines) {
		glVertex3i(a.pointOne.x, a.pointOne.y, a.pointOne.z);
		glVertex3i(a.pointTwo.x, a.pointTwo.y, a.pointTwo.z);
	}
	glEnd();
	glPopMatrix();
}
GLint cube::getCurrentRotation() {
	return currentRotation;
}
GLint cube::getCurrentTranslationX() {
	return currentTranslationX;
}
GLint cube::getCurrentTranslationY() {
	return currentTranslationY;
}
pt cube::getCentroid() {
	return centroid;
}
vector<pt> cube::getIndices() {
	return indices;
}
vector<lineSegment> cube::getLines() {
	return lines;
}
GLint cube::getCurrentTranslationZ() {
	return currentTranslationZ;
}
