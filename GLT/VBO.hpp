#pragma once
#include <vector>
#include <iostream>
#include <GL\glew.h>

namespace glt {
	class VBO {
	public:
		VBO() { }
		VBO(size_t size) { init(size); }
		~VBO() { dispose(); }

		void init(size_t);

		void dispose() { glDeleteVertexArrays(1, &handle); }

		void bind();
		void setData(std::vector<float>);
		void setData(float*, size_t);
		void fillData(float, size_t);

		void setSize(size_t size) { this->size = size; }
		size_t getSize() { return size; }

		GLuint getHandle() const { return handle; }

		operator GLuint() const { return getHandle(); }

	private:
		GLuint handle = -1;
		size_t size = 3;

		std::vector<float> data;
	};
}