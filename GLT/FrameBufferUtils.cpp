#include "FrameBufferUtils.hpp"

using namespace glt;
using namespace glm;

const char* FBU_VERT =
"#version 330 core\n"

"in vec2 a_position;"
"in vec2 a_uv;"

"out vec2 f_uv;"

"void main() {"
"	gl_Position = vec4(a_position, 0.0, 1.0);"
"	f_uv = a_uv;"
"}";

const char* FBU_FRAG =
"#version 330 core\n"

"uniform sampler2D u_sampler;"

"in vec2 f_uv;"

"out vec4 outColor;"

"void main() {"
"	outColor = texture(u_sampler, f_uv);"
"}";

Shader* FrameBufferUtils::displayShader = nullptr;
Mesh* FrameBufferUtils::displayMesh = nullptr;

void FrameBufferUtils::InitDisplay() {
	if (displayShader != nullptr)
		return;

	displayShader = new Shader(FBU_VERT, FBU_FRAG);

	displayMesh = new Mesh();
	displayMesh->getVertexVBO()->setDataSize(2);

	float quad[]{
		-1.f, -1.f,
		1.f, -1.f,
		-1.f, 1.f,

		1.f, -1.f,
		-1.f, 1.f,
		1.f, 1.f
	};

	float uv[]{
		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 0.f,
		0.f, 1.f,
		1.f, 1.f
	};

	displayMesh->setVertices(quad, sizeof(quad));
	displayMesh->setUVS(uv, sizeof(uv));

	displayMesh->getVAO()->bindBufferToAttr(displayMesh->getVertexVBO(), displayShader->getAttrib("a_position"));
	displayMesh->getVAO()->bindBufferToAttr(displayMesh->getUvVBO(), displayShader->getAttrib("a_uv"));
}

void FrameBufferUtils::Display(Texture* buffer) {
	InitDisplay();

	buffer->bind();
	displayShader->use();
	displayMesh->draw();
}

void FrameBufferUtils::Display(Texture* buffer, vec2 min, vec2 max) {
	InitDisplay();

	//Set vertices
	float vertices[]{
		min.x, min.y,
		max.x, min.y,
		min.x, max.y,

		max.x, min.y,
		min.x, max.y,
		max.x, max.y
	};

	displayMesh->setVertices(vertices, sizeof(vertices));

	buffer->bind();
	displayShader->use();
	displayMesh->draw();
}