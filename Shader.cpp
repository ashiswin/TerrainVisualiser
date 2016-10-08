#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>

void Shader::addShader(GLenum shaderType, char* source) {
	FILE *sourceFile = fopen(source, "rb");
	fseek(sourceFile, 0, SEEK_END);
	long size = ftell(sourceFile);
	fseek(sourceFile, 0, SEEK_SET);
	
	char* shaderSource = (char*) malloc(size + 1);
	fread(shaderSource, size, 1, sourceFile);
	fclose(sourceFile);
	
	shaderSource[size] = 0;
	
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderSource, (const GLint*) &size);
	glCompileShader(shaderId);
	
	GLint success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		
		char* errorMessage = (char*) malloc(length);
		glGetShaderInfoLog(shaderId, length, &length, errorMessage);
		
		printf("Error compiling shader %s: %s\n", source, errorMessage);
		
		free(errorMessage);
		glDeleteShader(shaderId);
		free(shaderSource);
		
		return;
	}
	
	free(shaderSource);
	
	shaders.push_back(shaderId);
}

void Shader::compileShader() {
	programId = glCreateProgram();
	for(int i = 0; i < shaders.size(); i++) {
		glAttachShader(programId, shaders.at(i));
	}
	
	glLinkProgram(programId);
	
	GLint success = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if(success == GL_FALSE) {
		GLint length = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		
		char* errorMessage = (char*) malloc(length);
		glGetProgramInfoLog(programId, length, &length, errorMessage);
		
		printf("Error linking shader: %s\n", errorMessage);
		
		free(errorMessage);
		return;
	}
}

void Shader::bind() {
	glUseProgram(programId);
}

void Shader::unbind() {
	glUseProgram(0);
}

int Shader::getUniformLocation(const char* uniform) {
	return glGetUniformLocation(programId, uniform);
}
