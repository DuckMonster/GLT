#include "VBO.hpp"
using namespace glt;

void VBO::init() {
	glGenBuffers(1, &handle);
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::setData(std::vector<float> data) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
}