#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "loadObjects.h"

obj* model::loadObjects(char *fileName, char * mtlname)
{
	obj* newobj = new obj();

	FILE *fp;
	errno_t err;
	char buffer[1024];
	int n = 0;
	int n_nom = 0;
	int t_nom = 0;
	int v[3];
	int vn[3];
	int vt[3];
	struct triangl tmp;
	struct texcoord ttmp;
	struct indexnum ftmp;

	strcpy_s(filename, sizeof(filename), fileName);
	err = fopen_s(&fp, fileName, "r");
	strcpy_s(mtllib, sizeof(mtllib), mtlname);

	if (err == 0)		printf("open success!\n");
	else
	{
		printf("error!\n");

	}

	while (fscanf_s(fp, "%s", buffer, sizeof(buffer)) != EOF) {


		if (!strcmp("mtllib", buffer)) {

			loadMaterialLib(fp);

		}
		else if (!strcmp("v", buffer)) {

			tmp.fnum = n;
			fscanf_s(fp, "%f %f %f", &tmp.x, &tmp.y, &tmp.z);
			newobj->triangls.push_back(tmp);
			n++;

		}
		else if (!strcmp("vt", buffer)) {

			tmp.fnum = t_nom;
			fscanf_s(fp, "%f %f", &ttmp.u, &ttmp.v);
			newobj->texcoords.push_back(ttmp);
			t_nom++;

		}

		else if (!strcmp("vn", buffer)) {

			tmp.fnum = n_nom;
			fscanf_s(fp, "%f %f %f", &tmp.x, &tmp.y, &tmp.z);
			newobj->triangl_nomal.push_back(tmp);
			n_nom++;

		}
		else if (!strcmp("f", buffer)) {

			
			fgets(buffer, 1024, fp);
			char *p = buffer;
			for (int i = 0; i < 3; i++)
			{
				while (*p == ' ')
				{
					p++;
				}
				if (*p == '\0' || *p == '\n')
				{
					break;
				}

				v[i] = strtoul(p, &p, 10);
				if (*p&&*p == '/')
				{
					p++;

					if (*p&&*p == '/')//    f v//vn v//vn v//vn 
					{
						p++;
						vn[i] = strtoul(p, &p, 10);
					}
					else if (*p != '/'&& *p != ' ')//    f v/vt/vn v/vt/vn v/vt/vn
					{
						vt[i] = strtoul(p, &p, 10);
						
						if (*p == '/')
						{
							p++;
							vn[i] = strtoul(p, &p, 10);
						}
					}
				}
			}

			ftmp.one = v[0];
			ftmp.two = v[1];
			ftmp.three = v[2];
			ftmp.n_one = vn[0];
			ftmp.n_two = vn[1];
			ftmp.n_three = vn[2];
			ftmp.t_one = vt[0];
			ftmp.t_two = vt[1];
			ftmp.t_three = vt[2];


			newobj->indexs.push_back(ftmp);

		}
		else if (!strcmp("s", buffer))fscanf_s(fp, "%s", buffer, sizeof(buffer));
		
	}

	fclose(fp);
	printf("파일 다읽었음!");
	return newobj;

}

void model::loadBmpTexture(char* fileName, GLuint* texture)
{
	AUX_RGBImageRec* texRec;

	if ((texRec = LoadBMPFile(fileName))) {
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec->sizeX, texRec->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec->data);
	}

	if (texRec)
	{
		if (texRec->data) free(texRec->data);
		free(texRec);
	}
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void model::loadMaterials(FILE * file)
{

	char parameter[32];
	mnode * temp ;
	temp = &mydata;
	while (fscanf_s(file, "%s", parameter, sizeof(parameter)) != EOF)
	{

		if (!strcmp("newmtl", parameter))
		{
			temp = new mnode();
			fscanf_s(file, "%s", temp->data.name, sizeof(temp->data.name));

		}
		if (!strcmp("illum", parameter)) fscanf_s(file, "%i", &temp->data.illum, sizeof(temp->data.illum));
		if (!strcmp("map_Kd", parameter))
		{
			fscanf_s(file, "%s", temp->data.map_Kd, sizeof(temp->data.map_Kd));

			if (strstr(temp->data.map_Kd, ".bmp") != NULL)
				loadBmpTexture(temp->data.map_Kd, &(temp->data.texture));

		}
		if (!strcmp("Kd", parameter))
			fscanf_s(file, "%f %f %f", &temp->data.Kd[0], &temp->data.Kd[1], &temp->data.Kd[2], sizeof(temp->data.Kd[0]), sizeof(temp->data.Kd[1]), sizeof(temp->data.Kd[2]));
		if (!strcmp("Ka", parameter))
			fscanf_s(file, "%f %f %f", &temp->data.Ka[0], &temp->data.Ka[1], &temp->data.Ka[2], sizeof(temp->data.Ka[0]), sizeof(temp->data.Ka[1]), sizeof(temp->data.Ka[2]));
		if (!strcmp("Ks", parameter))
			fscanf_s(file, "%f %f %f", &temp->data.Ks[0], &temp->data.Ks[1], &temp->data.Ks[2], sizeof(temp->data.Ks[0]), sizeof(temp->data.Ks[1]), sizeof(temp->data.Ks[2]));

	}

}
bool model::loadMaterialLib(FILE * file)
{

	FILE * lib;
	fopen_s(&lib, mtllib, "r");

	if (lib == NULL)
	{

		return false;
	}
	else loadMaterials(lib);
	fclose(lib);
	return true;
}




