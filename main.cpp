/**
* Project Name: Heightmap Based Terrain
* Author: ashiswin (Isaac Ashwin)
* File modified: 11.07.15
*/

#define GLM_SWIZZLE

#include <GL/glew.h>
#include <GL/glxew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "Shader.h"
#include "Terrain.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using std::vector;

const GLfloat PI = 3.14159265;

int screenWidth = 1920;
int screenHeight = 1080;
GLfloat speed = 3.0f;
GLfloat mouseSpeed = 0.0005f;
GLfloat horizontalAngle = 0;
GLfloat verticalAngle = 0.0f;

bool quit = false;
SDL_Event event;

void init();
void reshape(int, int);
void display();
void input();

SDL_Window *screen;
SDL_GLContext context;
GLfloat rot;
glm::vec3 position(0.0f, 100.0f, 0.0f);
glm::vec3 direction(0.0f, 0.0f, -1.0f);
glm::vec3 right(1.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f,  1.0f, 0.0f);
long lastTime = 0, currentTime = 0;

glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 translatedViewMatrix;
glm::mat4 modelMatrix(1.0f);
glm::vec2 heightmapDimensions;

int projectionMatrixLocation;
int viewMatrixLocation;
int modelMatrixLocation;
int heightmapDimensionsLocation;

Shader shader;
Terrain *terrain;

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	screen = SDL_CreateWindow("Terrain (Camera Controlled)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	if(screen == NULL) {
		printf("Could not create screen: %s\n", SDL_GetError());
	}
	
	context = SDL_GL_CreateContext(screen);
	
	glewExperimental = GL_TRUE;
	int error = glewInit();
	if(error != GLEW_OK) {
		printf("Glew failed to init: %s\n", glewGetErrorString(error));
		exit(1);
	}
	glGetError(); // Clear INVALID_ENUM error from glew

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	//glLineWidth(2.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	reshape(screenWidth, screenHeight);

	srand(SDL_GetTicks());
	
	shader.addShader(GL_VERTEX_SHADER, "shader.vert");
	shader.addShader(GL_FRAGMENT_SHADER, "shader.frag");
	shader.compileShader();
	
	terrain = new Terrain();
	terrain->setHeightMap("heightmap.jpg");
	
	heightmapDimensions = glm::vec2(TERRAINWIDTH, TERRAINHEIGHT);
	projectionMatrixLocation = shader.getUniformLocation("projectionMatrix");
	viewMatrixLocation = shader.getUniformLocation("viewMatrix");
	modelMatrixLocation = shader.getUniformLocation("modelMatrix");
	heightmapDimensionsLocation = shader.getUniformLocation("heightmapDimensions");
	
	printf("Projection Location: %d\nView Location: %d\nModel Location: %d\n", projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation);
}

void reshape(int screenW, int screenH) {
	screenWidth = screenW;
	screenHeight = screenH;

	glViewport(0, 0, screenW, screenH);
	projectionMatrix = glm::perspective(45.0f, (float)1920/1080, 0.1f, 10000.f);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
}

void display() {
		currentTime = SDL_GetTicks();
		lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.bind();
	
	modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
	translatedViewMatrix = glm::lookAt(position, position + direction, up);
	
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &translatedViewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniform2fv(heightmapDimensionsLocation, 1, &heightmapDimensions[0]);
	
	terrain->drawTerrain();
	
	shader.unbind();
	
	SDL_GL_SwapWindow(screen);
}

void input() {
	while (SDL_PollEvent(&event)) {
		float deltaTime = SDL_GetTicks() / 1000;
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_LEFT:
						rot += 5.0f;
						break;
					case SDLK_RIGHT:
						rot -= 5.0f;
						break;
					case SDLK_w:
						position += direction * deltaTime * speed;
						break;
					case SDLK_s:
						position -= direction * deltaTime * speed;
						printf("%f %f %f\n", position.x, position.y, position.z);
						printf("%d %d\n", deltaTime, speed);
						break;
					case SDLK_UP:
						position += up * deltaTime * speed;
						break;
					case SDLK_DOWN:
						position -= up * deltaTime * speed;
						break;
					case SDLK_a:
						position -= right * deltaTime * speed;
						break;
					case SDLK_d:
						position += right * deltaTime * speed;
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				int x, y;
				SDL_GetMouseState(&x, &y);
				SDL_WarpMouseInWindow(screen, screenWidth/2, screenHeight/2);
				horizontalAngle += mouseSpeed * deltaTime * float(screenWidth/2 - x);
				verticalAngle += mouseSpeed * deltaTime * float(screenHeight/2 - y);
				
				if(verticalAngle >= PI / 2) verticalAngle = PI / 2;
				if(verticalAngle <= -PI / 2) verticalAngle = -PI / 2;
				
				direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
				right = glm::vec3(sin(horizontalAngle - PI/2), 0, cos(horizontalAngle - PI/2));
				up = glm::cross(right, direction);
				break;
		}
	}
}

int main(int argc, char** argv) {
	printf("Begin initialization...\n");
	init();
	
	SDL_WarpMouseInWindow(screen, screenWidth/2, screenHeight/2);
	rot = 0.0f;
	
	printf("Initialization complete\n");
	
	while (!quit) {
		input();
		display();
	}

	SDL_Quit();

	return 0;
}
