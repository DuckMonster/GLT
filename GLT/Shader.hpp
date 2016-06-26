#pragma once
#include<string>
#include<set>
#include<map>
#include<glm\glm.hpp>
#include<GL\glew.h>

namespace glt {
	class Shader {
	public:
		static std::string ReadFile(std::string filename);

		//-------------

		bool uniformNotFoundWarning = true;

		//-------------

		Shader() {};
		Shader(std::string, std::string);
		~Shader() { dispose(); }

		void compile(std::string, std::string);
		void dispose();

		void use();
		GLuint getUniform(const char* name);
		GLuint getAttrib(const char* name);

		bool set(const char*, bool);
		bool set(const char*, float);
		bool set(const char*, int);
		bool set(const char*, glm::vec2);
		bool set(const char*, glm::vec3);
		bool set(const char*, glm::vec4);
		bool set(const char*, glm::mat2);
		bool set(const char*, glm::mat3);
		bool set(const char*, glm::mat4);

		operator GLuint() const { return program; }

	private:
		static GLuint activeProgram;
		GLuint program = -1;

		std::set<std::string> uniformErrorSet;

		GLuint createShader(GLenum, const char*);
	};
}