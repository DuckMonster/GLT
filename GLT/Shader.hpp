#pragma once
#include<string>
#include<GL\glew.h>

namespace glt {
	class Shader {
	public:
		static std::string ReadFile(const char* filename);

		Shader(const char*, const char*);

		void use();
		GLuint getUniform(const char*);

		operator GLuint() const { return program; }

	private:
		GLuint program, vertex, fragment;

		void init(const char*, const char*);
		GLuint createShader(GLenum, const char*);
	};
}