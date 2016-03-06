#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <iomanip>

#include "Mesh.h"

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
	float sinx = sin(glm::radians(rotation));
	float cosx = cos(glm::radians(rotation));

	glm::mat4 model = glm::mat4(
		cosx * scale.x, -sinx * scale.y, 0, position.x,
		sinx * scale.x, cosx * scale.y, 0, position.y,
		0, 0, scale.z, position.z,
		0, 0, 0, 1
		);

	return transpose(model);
}

void Mesh::reset() {
	rotation = 0.f;
	scale = vec3(1.f);
	position = vec3(0.f);
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

void Mesh::setVertices(void* vertices, int size) {
	vbo_vertices.setData(vertices, size);
}

void Mesh::setUVS(void* uvs, int size) {
	vbo_uvs.setData(uvs, size);
}

void Mesh::setColors(void* colors, int size) {
	vbo_colors.setData(colors, size);
}

void Mesh::updateUniforms() {
	glUniformMatrix4fv(shader->getUniform("u_model"), 1, false, glm::value_ptr(getModelMatrix()));
}

void Mesh::draw() {
	shader->use();
	updateUniforms();

	vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}