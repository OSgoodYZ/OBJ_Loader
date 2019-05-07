#include <string>
#include <iostream>
#include <cmath>
#include "trackball.h"
#include <Eigen/Dense>
#include <GL/glut.h>

using namespace Eigen;
using namespace std;

#define m_pi 3.141592

void trackball_coordi(float input_x, float input_y, float width, float height, Vector3f &v)
{//projection
	float xy_dis, nomal_tmp;
	float temp3;
	v[0] = (2 * input_x - width) / width;
	v[1] = (height - 2 * input_y) / height;
	xy_dis = (float)sqrt((v[0] * v[0]) + (v[1] * v[1]));

	v[2] = cos(m_pi * 0.5 * ((xy_dis < 1.0) ? xy_dis : 1.0));//need 수정

	normalize(v);
	return;
}

void normalize(Vector3f &v)
{
	float d = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= d;
	v[1] *= d;
	v[2] *= d;
	return;
}
//camera============================================
void Camera::init()
{
	axisAngle = 0;
	axis << 0, 0, 1;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i == j)
				matrix[i * 4 + j] = 1;
			else
				matrix[i * 4 + j] = 0;

	eye << 0, 0, 1;
	at << 0, 0, 0;
	up << 0, 1, 0;

	float   aspectRatio;
	float   dNear;
	float   dFar;

}

void Camera::apply(Vector3f	&eye, Vector3f	&at, Vector3f	&up) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye(0), eye(1), eye(2),
		at(0), at(1), at(2),
		up(0), up(1), up(2));


}
//mouse==============================================
void mouse::init()
{
	lastpos << 0, 0, 0;

	trackingmouse = false;
	redrawcontinue = false;

}

void mouse::startmotion(int x, int y, float windowWidth, float windowHeight, float &axisAngle)
{
	trackingmouse = true;
	redrawcontinue = false;
	startx = x;
	starty = y;
	trackball_coordi(x, y, windowWidth, windowHeight, lastpos);//마지막지점을 현재점으로 지정
	axisAngle = 0.0f;
}

void mouse::stopmotion(int x, int y)
{
	trackingmouse = true;
	if (startx != x || starty != y) {
		redrawcontinue = true;
	}
	else {

		redrawcontinue = false;
	}

}