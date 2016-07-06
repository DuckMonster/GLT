#include <iostream>
#include <fstream>
#include <string>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.hpp"
using namespace std;
using namespace glt;
using namespace glm;

GLuint Shader::activeProgram = -1;

string Shader::ReadFile(std::string filename) {
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

Shader::Shader(string vertexSrc, string fragmentSrc) {
	compile(vertexSrc, fragmentSrc);
}

void Shader::dispose() {
	if (program == -1)
		return;

	glDeleteProgram(program);
	program = -1;
}

void Shader::compile(string vertexSrc, string fragmentSrc) {
	//Dispose shader in case it already compiled something else
	dispose();

	//Init
	program = glCreateProgram();
	GLuint vertex = createShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	GLuint fragment = createShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

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
	if (program == activeProgram)
		return;

	glUseProgram(program);
	activeProgram = program;
}

GLuint Shader::getUniform(const char* name) {
	GLuint uniformPos = glGetUniformLocation(program, name);

	//The uniform doesn't exist, but hasn't been tried before
	if (uniformPos == -1 && uniformNotFoundWarning && uniformErrorSet.find(name) == uniformErrorSet.end()) {
		cout << "Couldn't find uniform " << name << "!\n";
		uniformErrorSet.insert(name);
	}

	return uniformPos;
}

GLuint Shader::getAttrib(const char* name) {
	GLuint attribPos = glGetAttribLocation(program, name);

	if (attribPos == -1 && attributeErrorSet.find(name) == attributeErrorSet.end()) {
		cout << "Couldn't find attribute " << name << "!\n";
		attributeErrorSet.insert(name);
	}

	return attribPos;
}

//Uniform sets
//BOOL
bool Shader::set(const char* name, bool value) { return set(name, value ? 1 : 0); }

//FLOAT
bool Shader::set(const char* name, float value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniform1f(uniformLocation, value);
	return true;
}
//INTEGER
bool Shader::set(const char* name, int value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniform1i(uniformLocation, value);
	return true;
}

//VECTORS
bool Shader::set(const char* name, vec2 value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniform2f(uniformLocation, value.x, value.y);
	return true;
}
bool Shader::set(const char* name, vec3 value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniform3f(uniformLocation, value.x, value.y, value.z);
	return true;
}
bool Shader::set(const char* name, vec4 value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
	return true;
}

//MATRICES
bool Shader::set(const char* name, mat2 value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniformMatrix2fv(uniformLocation, 1, false, value_ptr(value));
	return true;
}
bool Shader::set(const char* name, mat3 value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniformMatrix3fv(uniformLocation, 1, false, value_ptr(value));
	return true;
}
bool Shader::set(const char* name, mat4 value) {
	GLuint uniformLocation = getUniform(name);
	if (uniformLocation == -1)
		return false;

	use();
	glUniformMatrix4fv(uniformLocation, 1, false, value_ptr(value));
	return true;
}