#pragma once
#include<string>
#include<GL\glew.h>

namespace glt {
	class Shader {
	public:
		static std::string ReadFile(const char* filename);
		Shader(const char*, const char*);

		operator GLuint() const { return program; }

	private:
		void init(const char*, const char*);
		void use();

		GLuint createShader(GLenum, const char*);
		GLuint getUniform(const char*);

		GLuint program, vertex, fragment;
	};
}