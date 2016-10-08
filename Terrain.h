#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_SWIZZLE
#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"

#define TERRAINWIDTH 2048
#define TERRAINHEIGHT 2048

class Terrain {
	private:
		GLuint vao;
		GLuint vertexBuffer, indexBuffer;
		GLuint texture;
		
		GLfloat terrainSquare[TERRAINWIDTH * TERRAINHEIGHT * 3];
		GLuint indices[(TERRAINWIDTH - 1) * (TERRAINHEIGHT - 1) * 2 * 3];
	public:
		Terrain();
		void setHeightMap(char* filename);
		void drawTerrain();
		void drawWireframe();
		int heightmapWidth, heightmapHeight;
};

#endif
