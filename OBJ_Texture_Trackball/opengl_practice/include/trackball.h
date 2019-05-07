#pragma once

#include <Eigen/Dense>
#define bool int
#define false 0
#define true 1

using namespace Eigen;

struct position
{
	float x, y;
};
void trackball_coordi(float input_x, float input_y, float width, float height, Vector3f &v);
void normalize(Vector3f &v);

class Camera
{
public:
	float axisAngle;
	Vector3f axis;

	Vector3f	eye;
	Vector3f	at;
	Vector3f	up;
	float matrix[16];


	float   aspectRatio;
	float   dNear;
	float   dFar;
public:
	Camera() {};
	~Camera() {};
	void init();
	void apply(Vector3f	&a, Vector3f	&b, Vector3f	&c);

	void beginRotate(int x, int y) {};
	void Rotate() {};
	void endRotate(int x, int y) {};
	void zoom() {};



};

class mouse
{
public:

	int startx, starty;

	bool    trackingmouse;
	bool    redrawcontinue;

	Vector3f lastpos;

public:
	void init();
	void startmotion(int x, int y, float windowWidth, float windowHeight, float &axisAngle);
	void stopmotion(int x, int y);
};