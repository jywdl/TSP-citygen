/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah

	Purpose: Driver for 3D program to load .ply models 
	Usage:	
	===================================================== */

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/names.h>
#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>

#include "MyGLCanvas.h"

using namespace std;

class MyAppWindow;
MyAppWindow *win;

class MyAppWindow : public Fl_Window {
public:

	Fl_Slider* eyeXSlider;
	Fl_Slider* eyeYSlider;
	Fl_Slider* eyeZSlider;

	Fl_Slider* rotXSlider;
	Fl_Slider* rotYSlider;
	Fl_Slider* rotZSlider;
	Fl_Slider* scaleSlider;

	MyGLCanvas* canvas;

public:
	// APP WINDOW CONSTRUCTOR
	MyAppWindow(int W, int H, const char*L = 0);

	static void idleCB(void* userdata) {
		win->canvas->redraw();
		//win->rotXSlider->value(win->canvas->rotVec.x);
		//win->rotYSlider->value(win->canvas->rotVec.y);
		//win->rotZSlider->value(win->canvas->rotVec.z);
	}

private:
	// Someone changed one of the sliders
	static void toggleCB(Fl_Widget* w, void* userdata) {
		int value = ((Fl_Button*)w)->value();
		//printf("value: %d\n", value);
		*((int*)userdata) = value;
	}

	static void sliderFloatCB(Fl_Widget* w, void* userdata) {
		float value = ((Fl_Slider*)w)->value();
		//printf("value: %f\n", value);
		*((float*)userdata) = value;
	}

	static void radioButtonCB(Fl_Widget* w, void* userdata) {
		const char* value = ((Fl_Button*)w)->label();
		if (strcmp("Cube", value) == 0) {
			win->canvas->setShape(SHAPE_CUBE);
		}
		else if (strcmp("Cylinder", value) == 0) {
			win->canvas->setShape(SHAPE_CYLINDER);
		}
		else if (strcmp("Cone", value) == 0) {
			win->canvas->setShape(SHAPE_CONE);
		}
		else if (strcmp("Sphere", value) == 0) {
			win->canvas->setShape(SHAPE_SPHERE);
		}
		else if (strcmp("Special", value) == 0) {
			win->canvas->setShape(SHAPE_SPECIAL1);
		}
	}
};


MyAppWindow::MyAppWindow(int W, int H, const char*L) : Fl_Window(W, H, L) {
	begin();
	// OpenGL window

	canvas = new MyGLCanvas(10, 10, w() - 160, h() - 20);

	Fl_Pack* pack = new Fl_Pack(w() - 150, 30, 150, h(), "");
	pack->box(FL_DOWN_FRAME);
	pack->type(Fl_Pack::VERTICAL);
	pack->spacing(30);
	pack->begin();

	Fl_Pack* cameraPack = new Fl_Pack(w() - 100, 30, 100, h(), "Camera");
	cameraPack->box(FL_DOWN_FRAME);
	cameraPack->labelfont(1);
	cameraPack->type(Fl_Pack::VERTICAL);
	cameraPack->spacing(0);
	cameraPack->begin();


		Fl_Box *eyeXbox = new Fl_Box(0, 0, pack->w() - 20, 20, "EyeX");
		eyeXSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		eyeXSlider->align(FL_ALIGN_TOP);
		eyeXSlider->type(FL_HOR_SLIDER);
		eyeXSlider->bounds(-359, 359);
		eyeXSlider->step(1);
		eyeXSlider->value(canvas->eyePosition.x);
		eyeXSlider->callback(sliderFloatCB, (void*)(&(canvas->eyePosition.x)));

		Fl_Box *eyeYbox = new Fl_Box(0, 0, pack->w() - 20, 20, "EyeY");
		eyeYSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		eyeYSlider->align(FL_ALIGN_TOP);
		eyeYSlider->type(FL_HOR_SLIDER);
		eyeYSlider->bounds(-359, 359);
		eyeYSlider->step(1);
		eyeYSlider->value(canvas->eyePosition.y);
		eyeYSlider->callback(sliderFloatCB, (void*)(&(canvas->eyePosition.y)));

		Fl_Box *eyeZbox = new Fl_Box(0, 0, pack->w() - 20, 20, "EyeZ");
		eyeZSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		eyeZSlider->align(FL_ALIGN_TOP);
		eyeZSlider->type(FL_HOR_SLIDER);
		eyeZSlider->bounds(-359, 359);
		eyeZSlider->step(1);
		eyeZSlider->value(canvas->eyePosition.z);
		eyeZSlider->callback(sliderFloatCB, (void*)(&(canvas->eyePosition.z)));



		//slider for controlling rotation
		Fl_Box *rotXTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateX");
		rotXSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		rotXSlider->align(FL_ALIGN_TOP);
		rotXSlider->type(FL_HOR_SLIDER);
		rotXSlider->bounds(-359, 359);
		rotXSlider->step(1);
		rotXSlider->value(canvas->rotVec.x);
		rotXSlider->callback(sliderFloatCB, (void*)(&(canvas->rotVec.x)));

		Fl_Box *rotYTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateY");
		rotYSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		rotYSlider->align(FL_ALIGN_TOP);
		rotYSlider->type(FL_HOR_SLIDER);
		rotYSlider->bounds(-359, 359);
		rotYSlider->step(1);
		rotYSlider->value(canvas->rotVec.y);
		rotYSlider->callback(sliderFloatCB, (void*)(&(canvas->rotVec.y)));

		Fl_Box *rotZTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateZ");
		rotZSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		rotZSlider->align(FL_ALIGN_TOP);
		rotZSlider->type(FL_HOR_SLIDER);
		rotZSlider->bounds(-359, 359);
		rotZSlider->step(1);
		rotZSlider->value(canvas->rotVec.z);
		rotZSlider->callback(sliderFloatCB, (void*)(&(canvas->rotVec.z)));

		Fl_Box *scaleTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "Scale");
		scaleSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		scaleSlider->align(FL_ALIGN_TOP);
		scaleSlider->type(FL_HOR_SLIDER);
		scaleSlider->bounds(0.1, 2);
		scaleSlider->value(canvas->scale);
		scaleSlider->callback(sliderFloatCB, (void*)(&(canvas->scale)));

		cameraPack->end();

	pack->end();

	end();
}


/**************************************** main() ********************/
int main(int argc, char **argv) {
	win = new MyAppWindow(1100, 700, "TSP");
	win->resizable(win);
	Fl::add_idle(MyAppWindow::idleCB);
	win->show();
	return(Fl::run());
}