#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include "vertex.h"

using namespace std;

class Cube : public Shape {
public:
	Cube() {};
	~Cube() {};

	OBJ_TYPE getType() {
		return SHAPE_CUBE;
	}


	void draw(float sx, float sy, float sz, float lx, float ly, float lz) {
		glPushMatrix(); {
			glTranslatef(sx*lx, sy*ly, lz*(0.5+sz));
			drawCubeFace(sx, sy, sz, lx, ly, lz);
		}
		glPopMatrix();
		glPushMatrix(); {
			glTranslatef((0.5+sx)*lx, sy*ly, sz*lz);
			glRotatef(90, 0, 1, 0);
			drawCubeFace(sx, sy, sz, lz, ly, lx);
		}
		glPopMatrix();
		glPushMatrix(); {
			glTranslatef(sx*lx, sy*ly, (-0.5+sz)*lz);
			glRotatef(180, 0, 1, 0);
			drawCubeFace(sx, sy, sz, lx, ly, lz);
		}
		glPopMatrix();
		glPushMatrix(); {
			glTranslatef((-0.5+sx)*lx, sy*ly, sz*lz);
			glRotatef(270, 0, 1, 0);
			drawCubeFace(sx, sy, sz, lz, ly, lx);
		}
		glPopMatrix();
		glPushMatrix(); {
			glTranslatef(sx*lx, (0.5+sy)*ly, sz*lz);
			glRotatef(270, 1, 0, 0);
			drawCubeFace(sx, sy, sz, lx, lz, ly);
		}
		glPopMatrix();
		glPushMatrix(); {
			glTranslatef(sx*lx, (-0.5+sy)*ly, sz*lz);
			glRotatef(90, 1, 0, 0);
			drawCubeFace(sx, sy, sz, lx, lz, ly);
		}
		glPopMatrix();
	};


	void drawCubeFace(float sx, float sy, float sz, float lx, float ly, float lz) {
		float width = 1.0f / (float)m_segmentsX;
		float height = 1.0f / (float)m_segmentsY;
		float startX = -0.5f;
		float startY = -0.5f;

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < m_segmentsX; i++) {
			float xcoord = startX + (i*width);
			for (int j = 0; j < m_segmentsY; j++) {
				float ycoord = startY + (j*height);

				glNormal3f(0, 0, 1);
				glVertex3d(lx*xcoord, ly*ycoord, 0);
				glVertex3d(lx*(xcoord + width), ly*ycoord, 0);
				glVertex3d(lx*(xcoord + width), ly*(ycoord + height), 0);

				glNormal3f(0, 0, 1);
				glVertex3d(lx*(xcoord + width), ly*(ycoord + height), 0);
				glVertex3d(lx*xcoord, ly*(ycoord + height), 0);
				glVertex3d(lx*xcoord, ly*ycoord, 0);
			}
		}
		glEnd();
	}

private:
	
};

#endif