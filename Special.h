#ifndef SPECIAL_H
#define SPECIAL_H

#include "Shape.h"

class Special : public Shape {
public:
	Special() {};
	~Special() {};

	OBJ_TYPE getType() {
		return SHAPE_SPECIAL1;
	}

	void draw() {
		vector<vertex> bottom;
		vector<vertex> top;
		vector<vertex> normals;

		float r = 0.5f;
		float theta = 2*PI/m_segmentsX;
		float y = 1.0 / m_segmentsY;

		
		for (int i = 0; i < m_segmentsX; i++){
			vertex v1;
			vertex v2;
			vertex v3;
			vertex v4;

			
			for (int j = 0; j < m_segmentsY; j++){
				//top
				v1.x = (r - ((y * j)/2)) * cosf(i * theta);
				v1.y = (-0.5f+(y * j)) * 0.5 + 0.25;
				v1.z = (-0.5f + ((y * j)/2)) * sinf(i * theta);

				v2.x = (r - ((y * (j + 1))/2)) * cosf(i * theta);
				v2.y = (-0.5f+(y * (j + 1))) * 0.5 + 0.25f;
				v2.z = (-0.5f + ((y * (j + 1))/2)) * sinf(i * theta);

				v3.x = (r - ((y * j)/2)) * cosf((i + 1) * theta);
				v3.y = (-0.5f+(y * j)) * 0.5f + 0.25f;
				v3.z = (-0.5f + ((y * j)/2)) * sinf((i + 1) * theta);

				v4.x = (r - ((y * (j + 1))/2)) * cosf((i + 1) * theta);
				v4.y = (-0.5f+(y * (j + 1))) * 0.5 + 0.25f;
				v4.z = (-0.5f + ((y * (j + 1))/2)) * sinf((i + 1) * theta);
				
				top.push_back(v1);
				top.push_back(v3);
				top.push_back(v2);
				
				top.push_back(v4);
				top.push_back(v2);
				top.push_back(v3);

				// bottom

				v1.x = (r - ((y * j)/2)) * cosf(i * theta);
				v1.y = ((-0.5f+(y * j)) * 0.5 + 0.25) * -1.0f;
				v1.z = (-0.5f + ((y * j)/2)) * sinf(i * theta);

				v2.x = (r - ((y * (j + 1))/2)) * cosf(i * theta);
				v2.y = ((-0.5f+(y * (j + 1))) * 0.5 + 0.25f) * -1.0f;
				v2.z = (-0.5f + ((y * (j + 1))/2)) * sinf(i * theta);

				v3.x = (r - ((y * j)/2)) * cosf((i + 1) * theta);
				v3.y = ((-0.5f+(y * j)) * 0.5 + 0.25) * -1.0f;
				v3.z = (-0.5f + ((y * j)/2)) * sinf((i + 1) * theta);

				v4.x = (r - ((y * (j + 1))/2)) * cosf((i + 1) * theta);
				v4.y = ((-0.5f+(y * (j + 1))) * 0.5 + 0.25f) * -1.0f;
				v4.z = (-0.5f + ((y * (j + 1))/2)) * sinf((i + 1) * theta);
				
				bottom.push_back(v1);
				bottom.push_back(v3);
				bottom.push_back(v2);
				
				bottom.push_back(v4);
				bottom.push_back(v2);
				bottom.push_back(v3);

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

		for (int i = bottom.size() - 1; i >= 0 ; i--){
			vertex v1 = bottom[i];
			vertex n1 = normals[i];
			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(v1.x, v1.y, v1.z);
		}

		for (int i = 0; i < top.size(); i++){
			vertex v1 = top[i];
			vertex n1 = normals[i];
			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();
	};

	void drawNormal() {

		vector<vertex> bottom;
		vector<vertex> top;

		float r = 0.5f;
		float theta = 2*PI/m_segmentsX;
		float y = 1.0 / m_segmentsY;

		for (int i = 0; i < m_segmentsX; i++){
			vertex v1;
			vertex v2;
			for (int j = 0; j < m_segmentsY; j++){
				// top
				v1.x = (r - ((y * j)/2)) * cosf(i * theta);
				v1.y = (-0.5f+(y * j)) * 0.5 + 0.25;
				v1.z = (-0.5f + ((y * j)/2)) * sinf(i * theta);

				v2.x = ((r + 0.1) - ((y * j)/2)) * cosf(i * theta);
				v2.y = (-0.5f+(y * j)) * 0.5 + 0.25 + 0.075f;
				v2.z = (-0.6f + ((y * j)/2)) * sinf(i * theta);
				top.push_back(v1);
				top.push_back(v2);

				//bottom
				v1.x = (r - ((y * j)/2)) * cosf(i * theta);
				v1.y = -1 * ((-0.5f+(y * j)) * 0.5 + 0.25);
				v1.z = (-0.5f + ((y * j)/2)) * sinf(i * theta);

				v2.x = ((r + 0.1) - ((y * j)/2)) * cosf(i * theta);
				v2.y = -1 * ((-0.5f+(y * j)) * 0.5 + 0.25) - 0.075f;
				v2.z = (-0.6f + ((y * j)/2)) * sinf(i * theta);
				bottom.push_back(v1);
				bottom.push_back(v2);
			}
		}

		glBegin(GL_LINES);

		
		for (int i = 0; i < bottom.size(); i++){
			vertex v1 = bottom[i];
			glVertex3f(v1.x, v1.y, v1.z);
		}
		
		for (int i = 0; i < top.size(); i++){
			vertex v1 = top[i];
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();
	};

private:
};

#endif