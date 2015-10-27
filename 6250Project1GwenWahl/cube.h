#pragma once
#include <vector>
#include <GL\glut.h>
#include "stdafx.h"
//Point structure for use in cube
struct pt {
	GLint x, y, z;
};
//Face structure for use in cube
struct face {
	pt bLeft, bRight, tLeft, tRight;
};
struct lineSegment {
	pt pointOne, pointTwo;
};
class cube {
private: 
	std::vector<pt> indices;
	std::vector<lineSegment> lines;
	pt centroid;
	GLint currentRotation = 0;
	GLint currentTranslationX;
	GLint currentTranslationY;
	GLint currentTranslationZ;

public:
	cube(std::vector<pt> indices, std::vector<lineSegment> lines);
	~cube();
	void adjustCurrentTranslationX(int xOffset);
	void adjustCurrentTranslationY(int yOffset);
	void adjustCurrentTranslationZ(int zOffset);
	void adjustCurrentRotation(int rOffset);
	void display();
	void reset();
	GLint getCurrentRotation();
	GLint getCurrentTranslationX();
	GLint getCurrentTranslationY();
	GLint getCurrentTranslationZ();
	pt getCentroid();
	std::vector<pt> getIndices();
	std::vector<lineSegment> getLines();

};

