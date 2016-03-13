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

Mesh::Mesh(Shader* shader) {
	init();
	setShader(shader);

	reset();
}

//Transformations
mat4 Mesh::getModelMatrix() {
	//Rotation matrices
	float xc = cos(radians(rotation.x));
	float xs = sin(radians(rotation.x));

	float yc = cos(radians(rotation.y));
	float ys = sin(radians(rotation.y));

	float zc = cos(radians(rotation.z));
	float zs = sin(radians(rotation.z));

	mat4
		rotx(
			1, 0, 0, 0,
			0, xc, xs, 0,
			0, -xs, xc, 0,
			0, 0, 0, 1),

		roty(
			yc, 0, -ys, 0,
			0, 1, 0, 0,
			ys, 0, yc, 0,
			0, 0, 0, 1),

		rotz(
			zc, -zs, 0, 0,
			zs, zc, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

	mat4 translation(
		1, 0, 0, position.x,
		0, 1, 0, position.y,
		0, 0, 1, position.z,
		0, 0, 0, 1);

	mat4 scale(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);

	mat4 model = scale * rotz * rotx * roty * translation;
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
	init();

	vao.bindBufferToAttr(vbo_vertices, glGetAttribLocation(*shader, "v_position"));
	vao.bindBufferToAttr(vbo_uvs, glGetAttribLocation(*shader, "v_uv"));
	vao.bindBufferToAttr(vbo_colors, glGetAttribLocation(*shader, "v_color"));
}

void Mesh::setShader(Shader* shader) {
	if (shader == nullptr)
		return;

	this->shader = shader;
	bindBuffers();
}

void Mesh::setVertices(float* vertices, int size) {
	vbo_vertices.setData(vertices, size);
	vertex_count = vbo_vertices.getVertexCount();
}

void Mesh::setUVS(float* uvs, int size) {
	vbo_uvs.setData(uvs, size);
	vertex_count = vbo_vertices.getVertexCount();
}

void Mesh::setColors(float* colors, int size) {
	vbo_colors.setData(colors, size);
	vertex_count = vbo_vertices.getVertexCount();
}

void Mesh::fillVertices(size_t count) {
	vbo_vertices.setVertexCount(count, 0.f);
	vbo_colors.setVertexCount(count, 1.0f);
	vbo_uvs.setVertexCount(count, 1.f);
}

void Mesh::updateUniforms() {
	glUniformMatrix4fv(shader->getUniform("u_model"), 1, false, glm::value_ptr(getModelMatrix()));
	glUniform1i(shader->getUniform("u_usingTexture"), texture != nullptr ? 1 : 0);

	if (texture != nullptr)
		texture->bind();
}

void Mesh::draw() {
	shader->use();
	Camera::active->updateShader(*shader, "u_camera");

	updateUniforms();

	vao.bind();
	fillVertices(vertex_count);
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}