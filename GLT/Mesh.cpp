#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <iomanip>

#include "Mesh.hpp"
#include "Camera.hpp"

using namespace glt;
using namespace glm;
using namespace std;

Mesh::Mesh() {
	init();
}

void Mesh::init() {
	vbo_vertices.init(3);
	vbo_colors.init(4);
	vbo_uvs.init(2);
}

void Mesh::setDataUsage(GLenum usage) {
	vbo_vertices.dataUsage = usage;
	vbo_uvs.dataUsage = usage;
	vbo_colors.dataUsage = usage;
}

void Mesh::setVertices(const float* vertices, int size) {
	vbo_vertices.setData(vertices, size);
	vertex_count = vbo_vertices.getVertexCount();

	fillVertices(vertex_count);
}

void Mesh::setUVS(const float* uvs, int size) {
	vbo_uvs.setData(uvs, size);
	vertex_count = vbo_uvs.getVertexCount();

	fillVertices(vertex_count);
}

void Mesh::setColors(const float* colors, int size) {
	vbo_colors.setData(colors, size);
	vertex_count = vbo_colors.getVertexCount();

	fillVertices(vertex_count);
}

void Mesh::fillVertices(size_t count) {
	vbo_vertices.setVertexCount(count, 0.f);
	vbo_colors.setVertexCount(count, 1.f);
	vbo_uvs.setVertexCount(count, 1.f);
}

void Mesh::draw() {
	if (texture != nullptr)
		texture->bind();

	vao.bind();
	glDrawArrays(drawMode, 0, vertex_count);
}