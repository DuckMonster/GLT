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

void VBO::setData(const std::vector<float> data) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], dataUsage);

	this->data.assign(data.begin(), data.end());
}

void VBO::setData(const float* data, size_t size) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, dataUsage);

	this->data.assign(data, data + size / sizeof(float));
}

const std::vector<float> VBO::getData() {
	return data;
}

void VBO::setVertexCount(size_t count, float defValue) {
	count *= dataSize;

	if (count == data.size())
		return;

	if (count > data.size()) {
		data.insert(data.end(), count - data.size(), defValue);

		setData(data);
	}
	else {
		setData(&data[0], count * sizeof(float));
	}
}

size_t VBO::getVertexCount() {
	return data.size() / dataSize;
}