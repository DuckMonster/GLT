#pragma once
#include <GL\glew.h>
#include "VBO.hpp"

namespace glt {
	class VAO {
	public:
		VAO() { init(); }
		void bind() { glBindVertexArray(handle); }

		void bindBufferToAttr(VBO vbo, GLuint attribute);
		void bindBufferToAttr(GLuint vbo, GLuint size, GLuint attribute);

		operator GLuint() const { return handle; }

	private:
		GLuint handle;

		void init() { glGenVertexArrays(1, &handle); }
	};
}