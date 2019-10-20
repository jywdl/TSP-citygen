#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

using namespace std;

class Sphere : public Shape {
public:
	Sphere() {};
	~Sphere() {};

	OBJ_TYPE getType() {
		return SHAPE_SPHERE;
	}

	void draw(float sx, float sy, float sz, float lx, float ly, float lz) {

		vector<vertex> vertexList;


		float r = 0.5f;
		float phi = PI/m_segmentsY;
		float theta = 2*PI/m_segmentsX;
		for (int i = 0; i < m_segmentsX; i++){
			for (int j = 0; j < m_segmentsY; j++){
				vertex v1;
				v1.x = r * sinf(phi * j) * cosf(theta * i);
				v1.y = r * cosf(phi * j);
				v1.z = r * sinf(phi * j) * sinf(theta * i);

				vertex v2;
				v2.x = r * sinf(phi * (j+1)) * cosf(theta * i);
				v2.y = r * cosf(phi * (j+1));
				v2.z = r * sinf(phi * (j+1)) * sinf(theta * i);

				vertex v3;
				v3.x = r * sinf(phi * j) * cosf(theta * (i+1));
				v3.y = r * cosf(phi * j);
				v3.z = r * sinf(phi * j) * sinf(theta * (i+1));

				vertex v4;
				v4.x = r * sinf(phi * (j+1)) * cosf(theta * (i+1));
				v4.y = r * cosf(phi * (j+1));
				v4.z = r * sinf(phi * (j+1)) * sinf(theta * (i+1));

				v1.x = lx * v1.x + sx;
				v1.y = ly * v1.y + sy;
				v1.z = lz * v1.z + sz;

				v2.x = lx * v2.x + sx;
				v2.y = ly * v2.y + sy;
				v2.z = lz * v2.z + sz;

				v3.x = lx * v3.x + sx;
				v3.y = ly * v3.y + sy;
				v3.z = lz * v3.z + sz;

				v4.x = lx * v4.x + sx;
				v4.y = ly * v4.y + sy;
				v4.z = lz * v4.z + sz;

				vertexList.push_back(v1);
				vertexList.push_back(v2);
				vertexList.push_back(v3);
				vertexList.push_back(v2);
				vertexList.push_back(v4);
				vertexList.push_back(v3);
			}
		}
		glBegin(GL_TRIANGLES);

		for (int i = 0; i < vertexList.size(); i++){
			vertex v1 = vertexList[i];
			glNormal3f(v1.x, v1.y, v1.z);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();

	};

private:
};

#endif