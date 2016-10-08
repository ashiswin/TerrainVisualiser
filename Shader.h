#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/gl.h>

class Shader {
	public:
		void addShader(GLenum shaderType, char* source);
		void compileShader();
		void bind();
		void unbind();
		int getUniformLocation(const char* uniform);
	private:
		std::vector<GLuint> shaders;
		GLuint programId;
};

#endif
