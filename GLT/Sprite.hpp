#pragma once
#include <glm\glm.hpp>

#include "Mesh.hpp"
#include "Texture.hpp"

namespace glt {
	class Sprite {
	public:
		glm::vec2 position = glm::vec2(0.f);
		glm::vec2 scale = glm::vec2(0.f);
		float rotation = 0.f;

		glm::vec4 color = glm::vec4(1.f);

		Sprite(Texture*, Shader*);
		~Sprite();

		Mesh* getMesh() { return &mesh; }
		void setShader(Shader*);

		void reset();
		void setTexture(Texture*);
		void draw();

	private:
		static Shader* DEFAULT_SHADER;
		static void compileDefaultShader();

		Shader* shader;
		Mesh mesh;
		void initMesh();
		void updateMesh();
	};
}