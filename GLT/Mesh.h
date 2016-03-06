#pragma once
#include<glm\vec3.hpp>
#include<glm\matrix.hpp>
#include<vector>

#include "VAO.hpp"
#include "VBO.hpp"
#include "Shader.hpp"

namespace glt {
	class Mesh {
	public:
		glm::vec3 position;
		glm::vec3 scale;
		float rotation = 0.f;
		glm::vec3 rotationVec;

		Mesh() { };
		Mesh(Shader& shader);

		void init();

		void setShader(Shader& shader);
		void setVertices(void*, int);
		void setUVS(void*, int);
		void setColors(void*, int);

		void reset();

		glm::mat4 getModelMatrix();

		void draw();

	private:
		std::vector<glm::vec3> vertices;

		Shader* shader;

		VAO vao;
		VBO vbo_vertices, vbo_colors, vbo_uvs;

		void bindBuffers();
		void updateUniforms();
	};
}