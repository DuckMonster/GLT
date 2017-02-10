#pragma once
#include "FrameBuffer.hpp"
#include "Mesh.hpp"

namespace glt {
	class FrameBufferUtils {
	public:
		static Shader& GetQuadShader( );

	public:
		static void Quad();
		static void Quad(glm::vec2 min, glm::vec2 max);
		static void Quad(Shader& shader);
		static void Quad(Shader& shader, glm::vec2 min, glm::vec2 max);

	private:
		static Mesh* displayMesh;

		static void InitQuad();
	};
}