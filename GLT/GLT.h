#pragma once
#include "Shader.hpp"
#include "VBO.hpp"

namespace glt {
	void gltInit() {
		glewExperimental = true;
		glewInit();
	}
}