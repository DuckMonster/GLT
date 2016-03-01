#pragma once
#include <vector>
#include <GL\glew.h>

namespace glt {
	class VBO {
	private:
		void init();
		void bind();

		void setData(std::vector<float>);

		GLuint handle;
	};
}