#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape {
public:
	Cone() {};
	~Cone() {};

	OBJ_TYPE getType() {
		return SHAPE_CONE;
	}

	void draw(float sx, float sy, float sz, float lx, float ly, float lz) {
		vector<vertex> bottom;
		vector<vertex> sides;
		vector<vertex> normals;

		float r = 0.5f;
		float theta = 2*PI/m_segmentsX;
		float y = 1.0 / m_segmentsY;

		// bottom
		for (int i = 0; i < m_segmentsX; i++){
			vertex v1;
			vertex v2;
			vertex v3;
			vertex v4;

			// bottom face
			v1.x = 0;
			v1.z = -0.5;
			v1.y = 0;
			
			v2.x = r * cosf(i * theta);
			v2.z = -0.5;
			v2.y = r * sinf(i * theta);
			
			v3.x = r * cosf((i+1) * theta);
			v3.z = -0.5;
			v3.y = r * sinf((i+1) * theta);

			v1.x = lx * v1.x + sx;
			v1.y = ly * v1.y + sy;
			v1.z = lz * v1.z + sz;

			v2.x = lx * v2.x + sx;
			v2.y = ly * v2.y + sy;
			v2.z = lz * v2.z + sz;

			v3.x = lx * v3.x + sx;
			v3.y = ly * v3.y + sy;
			v3.z = lz * v3.z + sz;

			bottom.push_back(v1);
			bottom.push_back(v2);
			bottom.push_back(v3);

			//sides
			for (int j = 0; j < m_segmentsY; j++){
				v1.x = (r - ((y * j)/2)) * cosf(i * theta);
				v1.z = -0.5f+(y * j);
				v1.y = (-0.5f + ((y * j)/2)) * sinf(i * theta);

				v2.x = (r - ((y * (j + 1))/2)) * cosf(i * theta);
				v2.z = -0.5f+(y * (j + 1));
				v2.y = (-0.5f + ((y * (j + 1))/2)) * sinf(i * theta);

				v3.x = (r - ((y * j)/2)) * cosf((i + 1) * theta);
				v3.z = -0.5f+(y * j);
				v3.y = (-0.5f + ((y * j)/2)) * sinf((i + 1) * theta);

				v4.x = (r - ((y * (j + 1))/2)) * cosf((i + 1) * theta);
				v4.z = -0.5f+(y * (j + 1));
				v4.y = (-0.5f + ((y * (j + 1))/2)) * sinf((i + 1) * theta);

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
				
				sides.push_back(v1);
				sides.push_back(v3);
				sides.push_back(v2);
				
				sides.push_back(v4);
				sides.push_back(v2);
				sides.push_back(v3);

				vertex n1;
				n1.x = -cosf((i+r)*theta);
				n1.y = 0;
				n1.z =  -sinf((i+r)*theta);

				normals.push_back(n1);
				normals.push_back(n1);
				normals.push_back(n1);
				normals.push_back(n1);
				normals.push_back(n1);
				normals.push_back(n1);
			}

		}


		glBegin(GL_TRIANGLES);

		for (int i = 0; i < bottom.size(); i++){
			vertex v1 = bottom[i];
			glNormal3f(0, -1, 0);
			glVertex3f(v1.x, v1.y, v1.z);
		}

		for (int i = 0; i < sides.size(); i++){
			vertex v1 = sides[i];
			vertex n1 = normals[i];
			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();
	};

private:
};

#endif