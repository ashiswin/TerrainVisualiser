#include "Terrain.h"
#include "glm/gtc/matrix_transform.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

Terrain::Terrain() {
	for(int i = 0; i < TERRAINWIDTH * TERRAINHEIGHT * 3; i += 3) {
		int pos = i / 3;
		terrainSquare[i] = pos / TERRAINWIDTH;
		terrainSquare[i + 1] = pos % TERRAINWIDTH;
		terrainSquare[i + 2] = 0.0f;
	}
	for(int i = 0; i < (TERRAINWIDTH - 1) * (TERRAINHEIGHT - 1) * 2 * 3; i += 6) {
		int pos = i / 6;
		indices[i] = (int) (TERRAINWIDTH * floor(pos / (TERRAINWIDTH - 1))) + (pos % (TERRAINWIDTH - 1));
		indices[i + 1] = (int) (TERRAINWIDTH * (floor(pos / (TERRAINWIDTH - 1)) + 1)) + (pos % (TERRAINWIDTH - 1));
		indices[i + 2] = (int) (TERRAINWIDTH * floor(pos / (TERRAINWIDTH - 1))) + (pos % (TERRAINWIDTH - 1)) + 1;
		
		indices[i + 3] = indices[i + 1];
		indices[i + 4] = indices[i + 2];
		indices[i + 5] = indices[i + 1] + 1;
	}
	
	for(int i = 6; i < 12; i++) {
		printf("%d ", indices[i]);
		if(i % 3 == 2) printf("\n");
	}
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * TERRAINWIDTH * TERRAINHEIGHT * 3, terrainSquare, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (TERRAINWIDTH - 1) * (TERRAINHEIGHT - 1) * 2 * 3 * sizeof(GLuint), (GLuint*) indices, GL_STATIC_DRAW);
}

void Terrain::setHeightMap(char* filename) {
	SDL_Surface* heightmap = IMG_Load(filename);
	if(heightmap == NULL) {
		printf("Error: %s\n", SDL_GetError());
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, heightmap->w, heightmap->h, 0, GL_RGB, GL_UNSIGNED_BYTE, heightmap->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	for(int i = 0; i < 8; i++) {
		printf("%d ", ((char*)heightmap->pixels)[i]);
	}
	
	heightmapWidth = heightmap->w;
	heightmapHeight = heightmap->h;
	
	SDL_FreeSurface(heightmap);
}

void Terrain::drawTerrain() {
	glBindVertexArray(vao);
	
	glDrawElements(GL_TRIANGLES, (TERRAINWIDTH - 1) * (TERRAINHEIGHT - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

void Terrain::drawWireframe() {
	glBindVertexArray(vao);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, (TERRAINWIDTH - 1) * (TERRAINHEIGHT - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}
