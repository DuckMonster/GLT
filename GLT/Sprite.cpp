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

"out vec4 outColor;"

"void main() {"
"	vec4 color = vec4(1.0);"

"	if (u_usingTexture) {"
"		color = color * texture(u_texture, f_uv);"
"	}"

"	outColor = color * vec4(f_color, 1.0);"
"}";

Shader* Sprite::DEFAULT_SHADER = nullptr;
void Sprite::compileDefaultShader() {
	if (DEFAULT_SHADER != nullptr)
		return;

	DEFAULT_SHADER = new Shader(SPRITE_SRC_VERTEX, SPRITE_SRC_FRAGMENT);
}

Sprite::Sprite() : mesh() {
	Sprite::compileDefaultShader();
	mesh.setShader(*Sprite::DEFAULT_SHADER);

	initMesh();
}

Sprite::Sprite(Texture& texture) : mesh() {
	Sprite::compileDefaultShader();
	mesh.setShader(*Sprite::DEFAULT_SHADER);

	initMesh();
	setTexture(&texture);
}

Sprite::Sprite(Texture& texture, Shader& shader) : mesh(shader) {
	initMesh();
	setTexture(&texture);
}

Sprite::~Sprite() {
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

void Sprite::setTexture(Texture* texture) {
	mesh.texture = texture;

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
	mesh.draw();
}