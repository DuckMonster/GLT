#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

#include "Mesh.hpp"
#include "Camera.hpp"

using namespace glt;
using namespace glm;
using namespace std;

Mesh::Mesh() {
	init();
}

Mesh::Mesh(Primitive primitive) {
	init();
	loadPrimitive(primitive);
}

void Mesh::init() {
	vbo_vertices.setDataSize(3);
	vbo_colors.setDataSize(4);
	vbo_uvs.setDataSize(2);
	vbo_normals.setDataSize(3);
}

void Mesh::setDataUsage(GLenum usage) {
	vbo_vertices.dataUsage = usage;
	vbo_uvs.dataUsage = usage;
	vbo_colors.dataUsage = usage;
	vbo_normals.dataUsage = usage;
}

void Mesh::flipUV() {
	vector<float> uv = vbo_uvs.getData();
	for (size_t i = 0; i < uv.size(); i += 2) {
		//Flip Y element
		uv[i+1] = 1.f - uv[i+1];
	}

	setUVS(&uv[0], uv.size() * sizeof(float));
}

void Mesh::setVertices(const float* vertices, int size) {
	vbo_vertices.setData(vertices, size);
	vertex_count = vbo_vertices.getVertexCount();
}

void Mesh::setUVS(const float* uvs, int size) {
	vbo_uvs.setData(uvs, size);
	vertex_count = vbo_uvs.getVertexCount();
}

void Mesh::setColors(const float* colors, int size) {
	vbo_colors.setData(colors, size);
	vertex_count = vbo_colors.getVertexCount();
}

void Mesh::setNormals(const float* normals, int size) {
	vbo_normals.setData(normals, size);
	vertex_count = vbo_normals.getVertexCount();
}

void Mesh::fillVertices(size_t count) {
	vbo_vertices.setVertexCount(count, 0.f);
	vbo_colors.setVertexCount(count, 1.f);
	vbo_uvs.setVertexCount(count, 1.f);
	vbo_normals.setVertexCount(count, 0.f);
}

void Mesh::loadPrimitive(Primitive primitive) {
	switch (primitive) {
	case Quad: {
		float vertices[] {
			-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, 0.5f, 0.f,

			0.5f, -0.5f, 0.f,
			-0.5f, 0.5f, 0.f,
			0.5f, 0.5f, 0.f
		};
		float uvs[]{
			0.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,

			1.f, 0.f,
			0.f, 1.f,
			1.f, 1.f
		};
		float normals[] {
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,

			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
		};

		setVertices(vertices, sizeof(vertices));
		setUVS(uvs, sizeof(uvs));
		setNormals(normals, sizeof(normals));
	} break;

	case Triangle:
		float vertices[]{
			-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, 0.5f, 0.f
		};
		float uvs[]{
			0.f, 0.f,
			1.f, 0.f,
			0.f, 1.f
		};
		float normals[]{
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f,
			0.f, 0.f, -1.f
		};

		setVertices(vertices, sizeof(vertices));
		setUVS(uvs, sizeof(uvs));
		setNormals(normals, sizeof(normals));
	}
}

void Mesh::draw() {
	fillVertices(vertex_count);

	vao.bind();
	glDrawArrays(drawMode, 0, vertex_count);
}