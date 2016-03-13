#include <glm\gtc\type_ptr.hpp>
#include "Sprite.hpp"
using namespace glt;

const char* SPRITE_SRC_VERTEX =
"#version 450\n"
"in vec3 v_position;"
"in vec3 v_color;"
"in vec2 v_uv;"

"uniform mat4 u_camera;"
"uniform mat4 u_model;"

"out vec3 f_color;"
"out vec2 f_uv;"

"void main() {"
"	gl_Position = u_camera * u_model * vec4(v_position, 1.0);"
"	f_color = v_color;"
"	f_uv = v_uv;"
"}";

const char* SPRITE_SRC_FRAGMENT =
"#version 450\n"
"uniform sampler2D u_texture;"
"uniform bool u_usingTexture;"

"in vec3 f_color;"
"in vec2 f_uv;"

"uniform vec4 u_blendColor;"

"out vec4 outColor;"

"void main() {"
"	vec4 color = vec4(1.0);"

"	if (u_usingTexture) {"
"		color = color * texture(u_texture, f_uv);"
"	}"

"	outColor = color * vec4(f_color, 1.0) * u_blendColor;"
"}";

Shader* Sprite::DEFAULT_SHADER = nullptr;
void Sprite::compileDefaultShader() {
	if (DEFAULT_SHADER != nullptr)
		return;

	DEFAULT_SHADER = new Shader(SPRITE_SRC_VERTEX, SPRITE_SRC_FRAGMENT);
}

Sprite::Sprite(Texture* texture = nullptr, Shader* shader = nullptr) : mesh() {
	Sprite::compileDefaultShader();
	setShader(shader == nullptr ? Sprite::DEFAULT_SHADER : shader);

	initMesh();

	if (texture != nullptr)
		setTexture(texture);

	reset();
}

Sprite::~Sprite() {
}

void Sprite::setShader(Shader* shader) {
	this->shader = shader;
	mesh.setShader(shader);
}

void Sprite::initMesh() {
	float vertices[] = {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		-0.5f, 0.5f, 0.f,

		0.5f, -0.5f, 0.f,
		-0.5f, 0.5f, 0.f,
		0.5f, 0.5f, 0.f
	};
	float uvs[] = {
		0.f, 1.f,
		1.f, 1.f,
		0.f, 0.f,

		1.f, 1.f,
		0.f, 0.f,
		1.f, 0.f
	};

	mesh.setVertices(vertices, sizeof(vertices));
	mesh.setUVS(uvs, sizeof(uvs));
}

void Sprite::updateMesh() {
	mesh.position = glm::vec3(position, 0.f);
	mesh.scale = glm::vec3(scale, 1.f);
	mesh.rotation = glm::vec3(0.f, 0.f, rotation);
}

void Sprite::reset() {
	position = glm::vec2(0.f);
	scale = glm::vec2(1.f);
	rotation = 0.f;

	color = glm::vec4(1.f);
}

void Sprite::setTexture(Texture* texture) {
	mesh.texture = texture;
	if (texture == nullptr)
		return;

	float aspect = (float)texture->getWidth() / (float)texture->getHeight();
	float vertices[] = {
		-0.5f, -0.5f / aspect, 0.f,
		0.5f, -0.5f / aspect, 0.f,
		-0.5f, 0.5f / aspect, 0.f,

		0.5f, -0.5f / aspect, 0.f,
		-0.5f, 0.5f / aspect, 0.f,
		0.5f, 0.5f / aspect, 0.f
	};

	mesh.setVertices(vertices, sizeof(vertices));
}

void Sprite::draw() {
	shader->use();

	//Mesh uniforms
	updateMesh();
	//Color uniform
	glUniform4fv(shader->getUniform("u_blendColor"), 1, glm::value_ptr(color));

	mesh.draw();
}