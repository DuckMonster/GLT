#include "VAO.hpp"
#include "VBO.hpp"

using namespace glt;

void VAO::bindBufferToAttr(VBO& vbo, GLuint attribute) {
	bind();

	glEnableVertexAttribArray(attribute);

	vbo.bind();
	glVertexAttribPointer(attribute, vbo.getDataSize(), GL_FLOAT, false, 0, 0);
}

void VAO::bindBufferToAttr(GLuint vbo, GLuint size, GLuint attribute) {
	bind();
	glEnableVertexAttribArray(attribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute, size, GL_FLOAT, false, 0, 0);
}