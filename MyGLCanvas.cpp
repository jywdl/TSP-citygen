#include "MyGLCanvas.h"
#include <stdio.h> 
#include <cstdlib> 
#include <ctime>

//note: SIZE must be less than SQUARESIZE^2, DENSITY must be at least 2
#define SIZE 40
#define SQUARESIZE 10
#define DENSITY 3
#define TSP 0

Point shortestTour[SIZE];
Point path[SIZE];

int psize = SIZE;

float shortestTourLength;

int Shape::m_segmentsX;
int Shape::m_segmentsY;
int ro = 0;

vector<int> mask;
vector<int> hmask;
vector<int> lmask;
vector<int> btype;
//type 1 = cyl
//type 2 = box
//type 3 = connected
//type 4 = cross
vector<int> toptype;
//type 1 = flat
//type 2 = Cone
vector<int> touchmask;

MyGLCanvas::MyGLCanvas(int x, int y, int w, int h, const char *l) : Fl_Gl_Window(x, y, w, h, l) {
	mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	
	rotVec = glm::vec3(-40.0f, 0.0f, 53.0f);
	eyePosition = glm::vec3(0.0f, 0.0f, 6.0f);

	scale = 0.4f;

	objType = SHAPE_CUBE;
	cube = new Cube();
	cylinder = new Cylinder();
	cone = new Cone();
	sphere = new Sphere();
	special = new Special();
	shape = cube;
	srand(time(NULL));

	int check = 1;
	mask.push_back(1);
	hmask.push_back(rand() % 6 + 2);
	btype.push_back(rand() % 4 + 1);
	toptype.push_back(rand() % 3 + 1);
	lmask.push_back(rand() % 4 + 1);

	while(check < SIZE) {
		int num = rand() % DENSITY;
		lmask.push_back(rand() % 4 + 1);

		if(num != 0) {
			mask.push_back(0);
			hmask.push_back(0);
			btype.push_back(0);
			toptype.push_back(0);
		}
		else {
			int b = rand() % 4 + 1;
			if(b != 3) b = rand() % 4 + 1;
			if(b != 3) b = rand() % 4 + 1;

			int h = rand() % 6 + 2;


			if(b == 3) {
				int c = rand() % 4 + 1;
				int hh = rand() % (h - 1) + 1;

				h = 100*c + 10*hh + h;
			}

			mask.push_back(1);
			hmask.push_back(h);
			btype.push_back(b);

			int top = rand() % 2 + 1;
			int haspipe = rand() % 3;
			int xoffset = rand() % 10;
			int yoffset = rand() % 10;

			top = 1000*yoffset + 100*xoffset + 10*haspipe + top;
			toptype.push_back(top);

			check++;
		}
	}
}

MyGLCanvas::~MyGLCanvas() {
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete special;
}

void MyGLCanvas::setShape(OBJ_TYPE type) {
	objType = type;
	switch (type) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
		shape = cone;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	case SHAPE_SPECIAL1:
		shape = special;
		break;
	default:
		shape = cube;
	}
}

void MyGLCanvas::draw() {
	if (!valid()) {  //this is called when the GL canvas is set up for the first time or when it is resized...
		printf("establishing GL context\n");

		glViewport(0, 0, w(), h());
		updateCamera(w(), h());

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		GLfloat light_pos0[] = {eyePosition.x, eyePosition.y, eyePosition.z, 0.0f};
		GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };

		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		/****************************************/
		/*          Enable z-buferring          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(1, 1);
		glFrontFace(GL_CCW); //make sure that the ordering is counter-clock wise
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//rotate object
	glRotatef(rotVec.x, 1.0, 0.0, 0.0);
	glRotatef(rotVec.y, 0.0, 1.0, 0.0);
	glRotatef(rotVec.z, 0.0, 0.0, 1.0);

	glTranslatef(eyePosition.x/20, eyePosition.y/20, eyePosition.z/20);

	glScalef(scale, scale, scale);

	drawAxis();
	drawScene();
}

void MyGLCanvas::drawScene() {
	glPushMatrix();

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT, GL_FILL);

	int count = 0;
	for(int i = 0; i < SQUARESIZE; i+=1) {
		for(int j = 0; j < SQUARESIZE; j+=1) {
			glColor3f(0.5, 0.5, 0.5);

			float sx = 2*i;
			float sy = 2*j;

			if (mask[SQUARESIZE*i + j] == 1) {

				if(touchmask.size() < SQUARESIZE*SQUARESIZE)
					touchmask.push_back(1);

				//construct each type of building
				int h = hmask[SQUARESIZE*i + j];

				if(btype[SQUARESIZE*i + j] == 1) { //cylinder type
					h = h%10;
					shape = cylinder;
					shape->draw(sx, sy, h/2, 1, 1, h);
				}
				else if(btype[SQUARESIZE*i + j] == 2) { //box type
					h= h%10;
					shape = cube;
					shape->draw(sx, sy, 0.5, 1, 1, h);
				}
				else if(btype[SQUARESIZE*i + j] == 3) { // connected type 
					shape = cube;
					int subheight = (h/10) % 10;
					int dir = (h/100) % 10;
					h = h%10;

					switch (dir) {
						case 1:
						shape->draw(sx + 0.5, sy, 0.5, 1, 1, subheight); break;

						case 2:
						shape->draw(sx - 0.5, sy, 0.5, 1, 1, subheight); break;

						case 3:
						shape->draw(sx, sy + 0.5, 0.5, 1, 1, subheight); break;

						case 4:
						shape->draw(sx, sy - 0.5, 0.5, 1, 1, subheight); break;
					}
					shape->draw(sx, sy, 0.5, 1, 1, h);
				}
				else {
					shape = cube;
					shape->draw(sx, sy, 0.5, 1, 1, h);
					shape->draw(2*sx+1, 2*sy, 0.5, 0.5, 0.5, h-0.5);
					shape->draw(2*sx-1, 2*sy, 0.5, 0.5, 0.5, h-0.5);
					shape->draw(2*sx, 2*sy+1, 0.5, 0.5, 0.5, h-0.5);
					shape->draw(2*sx, 2*sy-1, 0.5, 0.5, 0.5, h-0.5);
				}

				//give some buildings a top
				int tt = toptype[SQUARESIZE*i + j];

				switch (tt % 10) {
					case 1: {

						shape = cube;
						if(btype[SQUARESIZE*i + j] == 1)
							shape->draw(0.8*sx, 0.8*sy, 4*h - 1, 1.25, 1.25, 0.25);
						else
							shape->draw(0.8*sx, 0.8*sy, 4*h, 1.25, 1.25, 0.25);

						shape = cylinder;

						int pipe = (tt/10) % 10;
						if(pipe != 0) {
							int xoffset = (tt/100) % 10;
							int yoffset = (tt/1000) % 10;

							float rx = static_cast<float>(xoffset)/10.0f - 0.5f;
							float ry = static_cast<float>(yoffset)/10.0f - 0.5f;

							glColor3f(0.2, 0.2, 0.2);
							shape->draw(sx+rx, sy+ry, h+0.1, 0.1, 0.1, 0.5); 
						}
						break;
					}

					case 2:	{
						shape = cone;
						if(btype[SQUARESIZE*i + j] == 1)
							shape->draw(sx, sy, h, 1, 1, 1);

						break;
					}
				}

				//fill TSP vertex
				if(!ro) {
					path[count][0] = sx;
					path[count][1] = sy;
					count++;
				}
			}
			else {
				//check adjacent plots, if there are 1-2 buildings do nothing, otherwise put land
				int north = SQUARESIZE*i + j + 1;
				int south = SQUARESIZE*i + j - 1;
				int east = SQUARESIZE*(i+1) + j;
				int west = SQUARESIZE*(i-1) + j;

				int touching = 0;
				if(north > 0 && mask[north] == 1)
					touching++;
				if(south > 0 && mask[south] == 1)
					touching++;
				if(east > 0 && mask[east] == 1)
					touching++;
				if(west > 0 && mask[west] == 1)
					touching++;

				if(touching > 2 || touching == 0) {
					fill_trees(sx, sy, lmask[SQUARESIZE*i + j]);
					if(touchmask.size() < SQUARESIZE*SQUARESIZE)
						touchmask.push_back(1);
				}
				else if(touchmask.size() < SQUARESIZE*SQUARESIZE)
					touchmask.push_back(0);
			}
		}	
	}

	//generate land
	for(int i = 0; i < SQUARESIZE; i++) {
		for(int j = 0; j < SQUARESIZE; j++) {

			glColor3f(0.2, 0.2, 0.2);

			float sx = 2*i;
			float sy = 2*j;

			if(touchmask[SQUARESIZE*i + j] == 1) {
				shape = cube;
				shape->draw(0.4*sx, 0.4*sy, 0, 2.5, 2.5, 0.5);
			}
			else {
				int north = SQUARESIZE*i + j + 1;
				int south = SQUARESIZE*i + j - 1;
				int east = SQUARESIZE*(i+1) + j;
				int west = SQUARESIZE*(i-1) + j;

				int touching = 0;
				if(north < 0 || touchmask[north] == 1)
					touching++;
				if(south < 0 || touchmask[south] == 1)
					touching++;
				if(east < 0 || touchmask[east] == 1)
					touching++;
				if(west < 0 || touchmask[west] == 1)
					touching++;

				if(touching == 4) {
					shape = cube;
					shape->draw(0.4*sx, 0.4*sy, 0, 2.5, 2.5, 0.5);
					fill_trees(sx, sy, lmask[SQUARESIZE*i + j]);
				}
			}
		}
	}
	
	if(!ro) {
		ro = 1;

		float dist[psize][psize];

		for(int i = 0; i < psize; i++)
			for(int j = 0; j < psize; j++) 
				dist[i][j] = computeDistance(path[i], path[j]);

		shortestTourLength = tour_length(path);
		copy_tour(shortestTour, path);

		if(TSP) scramble(path, path, psize);

		// printf("found the shortest tour:\n");
		// print_path(shortestTour);
		// printf("Length is: %f\n", shortestTourLength);

	}
	if(TSP) render_path(shortestTour);
	glPopMatrix();
}

void MyGLCanvas::fill_trees(float sx, float sy, int corner)
{
	shape = cylinder;

	int xt, yt;

	switch (corner) {
		case 1: xt = 1; yt = 1; break;
		case 2: xt = -1; yt = 1; break;
		case 3: xt = 1; yt = -1; break;
		case 4: xt = -1; yt = -1; break;
	}

	glColor3f(0.8, 0.6, 0.0);
	shape->draw(sx+0.4*xt, sy+0.4*yt, 0.5, 0.1, 0.1, 0.6); 
	shape = sphere;
	glColor3f(0.6, 1.0, 0.3);
	shape->draw(sx+0.4*xt, sy+0.4*yt, 0.8, 0.4, 0.4, 0.4);
}

void MyGLCanvas::render_path(Point points[])
{
	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glColor3f(1.0f, 0.41f, 0.71f);
	glPolygonMode(GL_FILL,GL_LINES);	
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);


    for (int i = 0; i < psize; i++) {
    	float x = points[i][0];
    	float y = points[i][1];
    	float z = 2.0f;
    	glVertex3f(x, y, z);
    }

	glEnd();
	glEnable(GL_LIGHTING);
}

//never called
void MyGLCanvas::print_path(Point points[])
{
    for (int i = 0; i < psize; i++) {
        printf("(%f,%f)", points[i][0], points[i][1]);
        if (i < psize - 1)
            printf(" , ");
    }
    printf("\n");
}

void MyGLCanvas::target_function(Point points[])
{
    float length;
    length = tour_length(points);
    if (length < shortestTourLength) {
        shortestTourLength = length;
        copy_tour(shortestTour, points);
    }
}

void MyGLCanvas::circ_perm(Point points[], int num)
{
	Point tmp;
	copy_point(tmp, points[0]);
	for (int i = 0; i < num - 1; i++)
		copy_point(points[i], points[i + 1]);

	copy_point(points[num - 1], tmp);
}

void MyGLCanvas::scramble(Point points[], Point *pivot, int num)
{
	Point *newPivot;
	if (num <= 1) { 
		target_function(points);
		return;
	}
	for (int i = 0; i < num; i++) {
		newPivot = &pivot[1];
		scramble(points, newPivot, num - 1);
		circ_perm(pivot, num);
	}
}

void MyGLCanvas::copy_tour(Point dest[], Point source[])
{	 
	for (int i = 0; i < psize; i++)
		copy_point(dest[i], source[i]);

}

void MyGLCanvas::copy_point(Point dest, Point source)
{
    dest[0] = source[0];
    dest[1] = source[1];
}

float MyGLCanvas::tour_length(Point points[])
{
    float length = 0;
    for(int i = 0; i < psize - 1; i++)
        length += computeDistance(points[i], points[i + 1]);

    length += computeDistance(points[psize - 1], points[0]);
    return length;
}

float MyGLCanvas::computeDistance(Point p, Point q)
{
	float xdiff = p[0] - q[0];
	float ydiff = p[1] - q[1];
	xdiff = xdiff * xdiff;
	ydiff = ydiff * ydiff;

	return sqrt(xdiff + ydiff);
}





int MyGLCanvas::handle(int e) {
	//printf("Event was %s (%d)\n", fl_eventnames[e], e);
	switch (e) {
	case FL_KEYUP:
		printf("keyboard event: key pressed: %c\n", Fl::event_key());
		break;
	case FL_MOUSEWHEEL:
		break;
	}

	return Fl_Gl_Window::handle(e);
}

void MyGLCanvas::resize(int x, int y, int w, int h) {
	Fl_Gl_Window::resize(x, y, w, h);
	puts("resize called");
}

void MyGLCanvas::drawAxis() {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();
	glEnable(GL_LIGHTING);
}

void MyGLCanvas::updateCamera(int width, int height) {
	float xy_aspect;
	xy_aspect = (float)width / (float)height;
	// Determine if we are modifying the camera(GL_PROJECITON) matrix(which is our viewing volume)
		// Otherwise we could modify the object transormations in our world with GL_MODELVIEW
	glMatrixMode(GL_PROJECTION);
	// Reset the Projection matrix to an identity matrix
	glLoadIdentity();
	gluPerspective(45.0f, xy_aspect, 0.1f, 10.0f);
	gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f);
}
