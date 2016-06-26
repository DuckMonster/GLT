#pragma once
#include "FrameBuffer.hpp"
#include "Mesh.hpp"

namespace glt {
	class FrameBufferUtils {
	public:
		static void Display(Texture* buffer);
		static void Display(Texture* buffer, glm::vec2 min, glm::vec2 max);

	private:
		static Shader* displayShader;
		static Mesh* displayMesh;

		static void InitDisplay();
	};
}