#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>
#include <glm/glm.hpp>
#include <time.h>
#include <iostream>

#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Special.h"
 
typedef float Point[2];

class MyGLCanvas : public Fl_Gl_Window {
public:

	glm::vec3 rotVec;
	glm::vec3 eyePosition;

	float scale;

	OBJ_TYPE objType;
	Cube* cube;
	Cylinder* cylinder;
	Cone* cone;
	Sphere* sphere;
	Special* special;
	Shape* shape;

	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();
	void setShape(OBJ_TYPE type);

private:
	void draw();
	void drawScene();

	void drawAxis();
	void fill_trees(float, float, int);

	float computeDistance(Point, Point);
	void copy_tour(Point [], Point []);
	void copy_point(Point, Point);

	void circ_perm(Point [], int);
	void target_function(Point []);
	void print_path(Point []);
	void render_path(Point []);


	float tour_length(Point []);
	void scramble(Point [], Point *, int);	//Generate all combination by brute force approach


	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);
};

#endif // !MYGLCANVAS_H