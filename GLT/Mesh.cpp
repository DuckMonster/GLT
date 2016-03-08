#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <iomanip>

#include "Mesh.hpp"

using namespace glt;
using namespace glm;
using namespace std;

Mesh::Mesh(Shader& shader) {
	init();
	setShader(shader);

	reset();
}

//Transformations
mat4 Mesh::getModelMatrix() {
	float a = cos(radians(rotation.x));
	float b = sin(radians(rotation.x));

	float x = cos(radians(rotation.y));
	float y = sin(radians(rotation.y));

	float u = cos(radians(rotation.z));
	float v = sin(radians(rotation.z));

	float i = scale.x;
	float j = scale.y;
	float k = scale.z;

	glm::mat4 model = glm::mat4(
		i*u*x, -a*j*v + b*j*u*y, -b*k*v - a*x*u*y, position.x,
		i*v*x, a*j*u + b*j*v*y, b*k*u - a*k*v*y, position.y,
		i*y, -b*j*x, a*k*x, position.z,
		0, 0, 0, 1
		);

	return transpose(model);
}

void Mesh::reset() {
	position = vec3(0.f);
	scale = vec3(1.f);
	rotation = vec3(0.f);
}
//----

void Mesh::init() {
	vbo_vertices.init(3);
	vbo_colors.init(3);
	vbo_uvs.init(2);
}

void Mesh::bindBuffers() {
	vao.bindBufferToAttr(vbo_vertices, glGetAttribLocation(*shader, "v_position"));
	vao.bindBufferToAttr(vbo_uvs, glGetAttribLocation(*shader, "v_uv"));
	vao.bindBufferToAttr(vbo_colors, glGetAttribLocation(*shader, "v_color"));
}

void Mesh::setShader(Shader& shader) {
	this->shader = &shader;
	bindBuffers();
}

void Mesh::setVertices(float* vertices, int size) {
	vbo_vertices.setData(vertices, size);
	vertex_count = size / sizeof(float);
}

void Mesh::setUVS(float* uvs, int size) {
	vbo_uvs.setData(uvs, size);
	vertex_count = size / sizeof(float);
}

void Mesh::setColors(float* colors, int size) {
	vbo_colors.setData(colors, size);
	vertex_count = size / sizeof(float);
}

void Mesh::fillVertices(size_t count) {
	vbo_vertices.setVertexCount(count, 0.f);
	vbo_colors.setVertexCount(count, 1.0f);
	vbo_uvs.setVertexCount(count, 1.f);
}

void Mesh::updateUniforms() {
	glUniformMatrix4fv(shader->getUniform("u_model"), 1, false, glm::value_ptr(getModelMatrix()));
}

void Mesh::draw() {
	shader->use();
	updateUniforms();

	vao.bind();
	fillVertices(vertex_count);
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}