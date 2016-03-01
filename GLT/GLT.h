#pragma once
#include "Shader.hpp"

namespace glt {
	void gltInit() {
		glewExperimental = true;
		glewInit();
	}
}