#include "FrameBuffer.hpp"

using namespace glt;

#pragma region
size_t FrameBuffer::clientWidth = 1024;
size_t FrameBuffer::clientHeight = 768;

Shader* FrameBuffer::display_shader = nullptr;
VAO* FrameBuffer::display_VAO = nullptr;
VBO* FrameBuffer::display_VBO_position = nullptr;
VBO* FrameBuffer::display_VBO_uv = nullptr;

const char* FRAMEBUFFER_SRC_VERTEX =
"#version 450\n"
"in vec2 v_position;"
"in vec2 v_uv;"

"out vec2 f_uv;"

"void main() {"
"	gl_Position = vec4(v_position, 0.0, 1.0);"
"	f_uv = v_uv;"
"}";

const char* FRAMEBUFFER_SRC_FRAGMENT =
"#version 450\n"
"uniform sampler2D u_texture;"
"in vec2 f_uv;"

"out vec4 outColor;"

"void main() {"
"	outColor = texture(u_texture, f_uv);"
"}";

void FrameBuffer::initDisplay() {
	if (display_shader != nullptr)
		return;

	float verts[] = {
		-1.f, -1.f,
		1.f, -1.f,
		1.f, 1.f,
		-1.f, 1.f
	};

	float uvs[] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};

	display_shader = new Shader(FRAMEBUFFER_SRC_VERTEX, FRAMEBUFFER_SRC_FRAGMENT);
	display_VAO = new VAO();
	display_VBO_position = new VBO(2);
	display_VBO_uv = new VBO(2);

	display_VBO_position->setData(verts, sizeof(verts));
	display_VBO_uv->setData(uvs, sizeof(uvs));

	display_VAO->bindBufferToAttr(display_VBO_position, display_shader->getAttrib("v_position"));
	display_VAO->bindBufferToAttr(display_VBO_uv, display_shader->getAttrib("v_uv"));
}
#pragma endregion Display

FrameBuffer::FrameBuffer(size_t width, size_t height) {
	this->width = width;
	this->height = height;

	init();
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &handle);
}

void FrameBuffer::init() {
	//Generate the framebuffer
	glGenFramebuffers(1, &handle);

	//Texture
	texture.bindToFramebuffer(this);

	//Render buffer
	renderBuffer.bindToFramebuffer(this);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glViewport(0, 0, width, height);
}

void FrameBuffer::display(Shader* shader) {
	if (shader == nullptr) {
		initDisplay();
		shader = display_shader;
	}

	display_VAO->bind();
	shader->use();
	texture.bind();

	glDrawArrays(GL_QUADS, 0, 4);
}