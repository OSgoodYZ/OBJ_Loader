#pragma once
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/GLAUX.h>
#include <vector>
#include <string>
#include <iostream>
//#include <ctype.h>
#include<stdlib.h>
//#include <Eigen/Dense>
#pragma comment(lib, "GLAUX.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

struct triangl
{//using when parsing vertex,normal information
	int fnum;
	float x, y, z;
};
struct texcoord
{
	float u, v;
	unsigned int index;
};
struct indexnum
{//using when parsing face index information
	int one, two, three;//vertex
	int t_one, t_two, t_three;
	int n_one, n_two, n_three;//vertex_normal
};

struct obj
{
	std::vector<triangl> triangls;//vertex information
	std::vector<texcoord> texcoords;
	std::vector<triangl> triangl_nomal;//nomal vector information
	std::vector<indexnum> indexs;//f's index
};

struct material
{
	char name[32];
	int illum;
	char map_Kd[32];
	float Kd[3] = { 0.8f, 0.8f, 0.8f };//initialization
	float Ka[3] = { 0.5f, 0.5f, 0.5f };
	float Ks[3] = { 0.1f, 0.0f, 0.0f };


	unsigned int texture;
};


class model
{
private:

	char mtllib[256];
	char filename[256];
	char directory[256];
	

	struct mnode
	{
		material data;
		//mnode * next;
	};

	AUX_RGBImageRec *LoadBMPFile(char *filename)
	{
		FILE *hFile = NULL;
		if (!filename) return NULL;
		fopen_s(&hFile, filename, "r");
		if (hFile) {
			fclose(hFile);
			return auxDIBImageLoad(filename);
		}

		return NULL;
	}

public:
	mnode mydata;

	obj* loadObjects(char *fileName, char * mtlname);

	bool loadMaterialLib(FILE * file);
	void loadMaterials(FILE * file);
	void loadBmpTexture(char* fileName, GLuint* texture);

};




