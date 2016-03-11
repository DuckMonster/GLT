#pragma once
#include<glm\vec3.hpp>
#include<glm\matrix.hpp>
#include<vector>

#include "VAO.hpp"
#include "VBO.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace glt {
	class Mesh {
	public:
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		Texture* texture = nullptr;

		Mesh() { };
		Mesh(Shader& shader);

		void init();

		void setShader(Shader& shader);
		void setVertices(float*, int);
		void setUVS(float*, int);
		void setColors(float*, int);

		void reset();

		glm::mat4 getModelMatrix();

		void draw();

	private:
		std::vector<glm::vec3> vertices;

		Shader* shader;

		VAO vao;
		VBO vbo_vertices, vbo_colors, vbo_uvs;
		std::size_t vertex_count;

		void bindBuffers();
		void updateUniforms();

		void fillVertices(size_t);
	};
}