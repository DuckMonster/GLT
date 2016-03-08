#include "VBO.hpp"
using namespace glt;

void VBO::init(size_t size) {
	if (handle != -1)
		return;

	setSize(size);
	glGenBuffers(1, &handle);
}

void VBO::bind() {
	if (handle == -1)
		throw std::exception("VBO not initialized");

	glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::setData(std::vector<float> data) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	this->data.assign(data.begin(), data.end());
}

void VBO::setData(float* data, size_t size) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	this->data.assign(data, data + size / sizeof(float));
}

void VBO::fillData(float data, size_t count) {
	float* dataArr = new float[count];
	for (size_t i = 0; i < count; i++)
		dataArr[i] = data;

	glBufferData(GL_ARRAY_BUFFER, count, dataArr, sizeof(float) * count);

	delete[] dataArr;
}