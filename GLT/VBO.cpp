#include "VBO.hpp"
using namespace glt;

void VBO::init(size_t size) {
	if (handle != -1)
		return;

	setDataSize(size);
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

void VBO::setVertexCount(size_t count, float defValue) {
	count *= dataSize;

	if (count == data.size())
		return;

	if (count > data.size()) {
		for (size_t i = data.size(); i < count; i++)
			data.push_back(defValue);

		setData(data);
	}
	else {
		setData(&data[0], count * sizeof(float));
	}
}

size_t VBO::getVertexCount() {
	return data.size() / dataSize;
}