#include <iostream>
#include <fstream>
#include <string>

#include "Shader.hpp"
using namespace std;
using namespace glt;

string Shader::ReadFile(const char* filename) {
	ifstream file(filename);
	string source = "";

	if (file.is_open()) {
		string line = "";

		while (getline(file, line)) {
			source += line + "\n";
		}

		file.close();
	}

	return source;
}

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
	init(vertexSrc, fragmentSrc);
}

void Shader::init(const char* vertexSrc, const char* fragmentSrc) {
	//Init
	program = glCreateProgram();
	GLuint vertex = createShader(GL_VERTEX_SHADER, vertexSrc);
	GLuint fragment = createShader(GL_FRAGMENT_SHADER, fragmentSrc);

	//Link program
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	//Cleanup shaders after linking
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
	glDeleteShader(vertex); 
	glDeleteShader(fragment);
}

GLuint Shader::createShader(GLenum type, const char* src) {
	//Init and compile
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	//Error check
	char buffer[1024];
	glGetShaderInfoLog(shader, 1024, nullptr, buffer);

	//... in case of error, print it to console
	if (strlen(buffer) > 0) {
		switch (type) {

		case GL_VERTEX_SHADER:
			cout << "[VERTEX]\n";
			break;

		case GL_FRAGMENT_SHADER:
			cout << "[FRAGMENT]\n";
			break;

		case GL_GEOMETRY_SHADER:
			cout << "[GEOMETRY]\n";
			break;

		}

		cout << buffer;
	}

	return shader;
}

void Shader::use() {
	glUseProgram(program);
}

GLuint Shader::getUniform(const char* name) {
	return glGetUniformLocation(program, name);
}