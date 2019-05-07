#include <windows.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>
#include <Eigen/Dense>
#pragma comment(lib, "glew32.lib")
#include <loadObjects.h>
#include <trackball.h>


using namespace std;
using namespace Eigen;

#define bool int
#define false 0
#define true 1

void init(void);
void ChangeSize(GLsizei w, GLsizei h);
void RenderScene();
void mouse_func(int button, int stat, int x, int y);
void drag_func(int x, int y);
void drawObj(obj*nowobj);

GLfloat windowWidth, windowHeight;
obj * nowobj = nullptr;
Camera cam;
mouse mice;

model myLoader;


void ChangeSize(GLsizei w, GLsizei h)
{

	glViewport(0, 0, w, h);//뷰포트를 창의 크기에 맞게 설정
	GLfloat aspectRatio;
	windowWidth = w;
	windowHeight = h;
	glMatrixMode(GL_PROJECTION);
	GLfloat ambientLight[] = { myLoader.mydata.data.Ka[0], myLoader.mydata.data.Ka[1],myLoader.mydata.data.Ka[2], 1.0f };
	GLfloat diffuseLight[] = { myLoader.mydata.data.Kd[0], myLoader.mydata.data.Kd[1], myLoader.mydata.data.Kd[2], 1.0f };
	GLfloat specularLight[] = { myLoader.mydata.data.Ks[0], myLoader.mydata.data.Ks[1], myLoader.mydata.data.Ks[2], 1.0f };

	GLfloat lightPos[] = { -10.0f, 10.0f, 10.0f, 0.0f };//2.0f, 2.0f, 1.0f, 1.0f
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLoadIdentity();

	glOrtho(-200, 200, -200, 200, -500, 500);
	//glOrtho(-3, 3, -3, 3, -3, 3);

	glMatrixMode(GL_MODELVIEW);

}
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(cam.eye[0], cam.eye[1], cam.eye[2],
		cam.at[0], cam.at[1], cam.at[2],
		cam.up[0], cam.up[1], cam.up[1]);
	glMultMatrixf(cam.matrix);

	drawObj(nowobj);

	glutSwapBuffers();//glFlush();

}


void init(void) {

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(cam.eye[0], cam.eye[1], cam.eye[2],
		cam.at[0], cam.at[1], cam.at[2],
		cam.up[0], cam.up[1], cam.up[1]);
	glLoadIdentity();

}
void drawObj(obj*nowobj)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(0, -100, 0);
	for (int i = 0; i < nowobj->indexs.size(); i++)
	{
		glColor3f(0, 0, 1);
		glBegin(GL_TRIANGLES);//GL_POLYGON//GL_POINTS//GL_TRIANGLES//

		glTexCoord2f(nowobj->texcoords[nowobj->indexs[i].t_one-1].u, nowobj->texcoords[nowobj->indexs[i].t_one - 1].v);
		glNormal3d(nowobj->triangl_nomal[nowobj->indexs[i].n_one - 1].x, nowobj->triangl_nomal[nowobj->indexs[i].n_one - 1].y, nowobj->triangl_nomal[nowobj->indexs[i].n_one - 1].z);
		glVertex3d(nowobj->triangls[nowobj->indexs[i].one - 1].x, nowobj->triangls[nowobj->indexs[i].one - 1].y, nowobj->triangls[nowobj->indexs[i].one - 1].z);


		glTexCoord2f(nowobj->texcoords[nowobj->indexs[i].t_two - 1].u, nowobj->texcoords[nowobj->indexs[i].t_two - 1].v);
		glNormal3d(nowobj->triangl_nomal[nowobj->indexs[i].n_two - 1].x, nowobj->triangl_nomal[nowobj->indexs[i].n_two - 1].y, nowobj->triangl_nomal[nowobj->indexs[i].n_two - 1].z);
		glVertex3d(nowobj->triangls[nowobj->indexs[i].two - 1].x, nowobj->triangls[nowobj->indexs[i].two - 1].y, nowobj->triangls[nowobj->indexs[i].two - 1].z);

		glTexCoord2f(nowobj->texcoords[nowobj->indexs[i].t_three - 1].u, nowobj->texcoords[nowobj->indexs[i].t_three - 1].v);
		glNormal3d(nowobj->triangl_nomal[nowobj->indexs[i].n_three - 1].x, nowobj->triangl_nomal[nowobj->indexs[i].n_three - 1].y, nowobj->triangl_nomal[nowobj->indexs[i].n_three - 1].z);
		glVertex3d(nowobj->triangls[nowobj->indexs[i].three - 1].x, nowobj->triangls[nowobj->indexs[i].three - 1].y, nowobj->triangls[nowobj->indexs[i].three - 1].z);

		glEnd();
	}

	glLoadIdentity();
}

void mouse_func(int button, int stat, int x, int y)
{
	if (button == GLUT_LEFT || button == GLUT_RIGHT_BUTTON)
	{
		if (stat == GLUT_DOWN)
		{
			mice.startmotion(x, y, windowWidth, windowHeight, cam.axisAngle);

		}
		else if (stat == GLUT_UP)
		{
			mice.stopmotion(x, y);
		}
	}

	if (stat == GLUT_UP)//mouse wheel
	{
		if (button == GLUT_WHEEL_UP)
		{
			glPushMatrix();
			glLoadIdentity();
			glScalef(1.1, 1.1, 1.1);
			glMultMatrixf(cam.matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)cam.matrix);
			glPopMatrix();
			glutPostRedisplay();
		}
		else if (button == GLUT_WHEEL_DOWN)
		{
			glPushMatrix();
			glLoadIdentity();
			glScalef(0.9, 0.9, 0.9);
			glMultMatrixf(cam.matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)cam.matrix);
			glPopMatrix();
			glutPostRedisplay();
		}
	}


}

void drag_func(int x, int y)
{
	Vector3f d;
	Vector3f curpos;
	trackball_coordi(x, y, windowWidth, windowHeight, curpos);
	if (mice.trackingmouse)
	{
		d = curpos - mice.lastpos;

		cam.axisAngle = 90.0f * sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);//calculate angle	

		//temp1 = sqrt(curpos[0] * curpos[0] + curpos[1] * curpos[1] + curpos[2] * curpos[2]);
		//temp2 = sqrt(mice.lastpos[0] * mice.lastpos[0] + mice.lastpos[1] * mice.lastpos[1] + mice.lastpos[2] * mice.lastpos[2]);
		//cam.axisAngle = 180.0f/3.14 * acos((mice.lastpos.dot(curpos)) / (temp1*temp2));

		cam.axis = mice.lastpos.cross(curpos);//cross product

		glPushMatrix();
		glLoadIdentity();
		glRotatef(cam.axisAngle, cam.axis[0], cam.axis[1], cam.axis[2]);
		glMultMatrixf(cam.matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)cam.matrix);
		glPopMatrix();
		mice.lastpos = curpos;

	}
	glutPostRedisplay();
}


int main(int argc, char **argv) {


	cam.init();
	mice.init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OBJ viewer!");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);


	//nowobj = myLoader.loadObjects((char*)"./sources\\tank\\BattleDroid.obj", (char*)"./sources\\tank\\BattleDroid.mtl");
	nowobj = myLoader.loadObjects((char*)"./sources\\bird\\Bloodwing.obj", (char*)"./sources\\bird\\Bloodwing.mtl");

	glutMotionFunc(drag_func);
	glutMouseFunc(mouse_func);

	init();
	glutMainLoop();

}