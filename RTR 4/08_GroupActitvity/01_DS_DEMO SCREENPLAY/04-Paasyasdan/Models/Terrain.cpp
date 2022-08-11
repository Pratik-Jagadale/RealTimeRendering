

#include "Terrain.h"
#include <stdio.h>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

//#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

const int TERRAIN_MAP_X = 257;
const int TERRAIN_MAP_Z = 257;
float MAX_HEIGHT_COLOR = 5.0f;
float HEIGHT_MAP_SCALE = 30.0f;
float xObj;
float yObj = -2.0f;

unsigned char *image_data = NULL;
GLuint texture_grass;
double terrainHeightMap[257][257][3];

Bool InItTerrain(void)
{
	if (loadGLTexture(&texture_grass, "./Models/Resources/grass.bmp") == False)
	{
		printf("Failure : texture_grass Texture.\n");
		return False;
	}

	if (loadHeightMapData("./Models/Resources/Height_Map.bmp") == False)
	{
		printf("Failure : loadHeightMapData Texture.\n");
		return False;
	}

	initializeTerrainCoords();
	return True;
}

void UnitiallizaTerrain(void)
{
	if (texture_grass)
	{
		glDeleteTextures(1, &texture_grass);
		texture_grass = 0;
	}

	if (image_data)
	{
		stbi_image_free(image_data);
		image_data = NULL;
	}
}

Bool loadHeightMapData(const char *filename)
{
	int w;
	int h;
	int channels_in_file;
	image_data = (unsigned char *)stbi_load(filename, &w, &h, &channels_in_file, 0);

	if (image_data == NULL)
	{
		printf("File loading Failed..,.\'n");
		return False;
	}

	return True;
}

void initializeTerrainCoords(void)
{
	int heightMultiplier = 2;
	for (int z = 0; z < TERRAIN_MAP_Z; z++)
	{

		for (int x = 0; x < TERRAIN_MAP_X; x++)
		{

			terrainHeightMap[x][z][0] = (float)(x)*HEIGHT_MAP_SCALE;
			// terrainHeightMap[x][z][1] = 0.0f;
			terrainHeightMap[x][z][1] = image_data[(x + z * TERRAIN_MAP_Z) * 3] * heightMultiplier;
			terrainHeightMap[x][z][2] = -(float)(z)*HEIGHT_MAP_SCALE;

			glEnd();
		}
	}
}
void drawTerrain(void)
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, texture_grass);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_2D, GL_MODULATE);
		for (int z = 0; z < (TERRAIN_MAP_Z - 1); z++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < (TERRAIN_MAP_X - 1); x++)
			{
				// glColor3f(1.0f, 1.0f, 1.0f);
				float scaleHeight = terrainHeightMap[x][z][1] / HEIGHT_MAP_SCALE;

				float nextScaleHeight = terrainHeightMap[x][z + 1][1] / HEIGHT_MAP_SCALE;

				float color = 0.5f + 0.5f * scaleHeight / MAX_HEIGHT_COLOR;

				float nextColor = 0.5f + 0.5f * nextScaleHeight / MAX_HEIGHT_COLOR;

				glTexCoord2f(0.0f, 0.0f);
				glColor3f(color, color, color);
				glVertex3f(terrainHeightMap[x][z][0], terrainHeightMap[x][z][1], terrainHeightMap[x][z][2]);

				glTexCoord2f(1.0f, 0.0f);
				glColor3f(color, color, color);
				glVertex3f(terrainHeightMap[x + 1][z][0], terrainHeightMap[x + 1][z][1], terrainHeightMap[x + 1][z][2]);

				glTexCoord2f(0.0f, 1.0f);
				glColor3f(nextColor, nextColor, nextColor);
				glVertex3f(terrainHeightMap[x][z + 1][0], terrainHeightMap[x][z + 1][1], terrainHeightMap[x][z + 1][2]);

				glTexCoord2f(1.0f, 1.0f);
				glColor3f(nextColor, nextColor, nextColor);
				glVertex3f(terrainHeightMap[x + 1][z + 1][0], terrainHeightMap[x + 1][z + 1][1], terrainHeightMap[x + 1][z + 1][2]);
			}
			glEnd();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}
