#pragma once
#include <vector>
#include <iostream>
#include <GL\glew.h>

namespace glt {
	class VBO {
	public:
		VBO() { init(); }
		~VBO() { dispose(); }

		void dispose() { glDeleteVertexArrays(1, &handle); }

		void bind();
		void setData(std::vector<float>);
		void setData(void*, unsigned int);

		void setSize(size_t size) { this->size = size; }
		size_t getSize() { return size; }

		GLuint getHandle() const { return handle; }

		operator GLuint() const { return getHandle(); }

	private:
		GLuint handle;
		size_t size = 3;

		void init();
	};
}