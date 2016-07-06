#pragma once
#include "FrameBuffer.hpp"
#include "Mesh.hpp"

namespace glt {
	class FrameBufferUtils {
	public:
		static void Quad();
		static void Quad(glm::vec2 min, glm::vec2 max);
		static void Quad(Shader* shader);
		static void Quad(Shader* shader, glm::vec2 min, glm::vec2 max);

	private:
		static Shader* displayShader;
		static Mesh* displayMesh;

		static void InitDisplay();
	};
}