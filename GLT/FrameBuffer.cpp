#include "FrameBuffer.hpp"

using namespace glt;
using namespace glm;

//Static members

ivec2 FrameBuffer::clientSize = vec2(0);
GLuint FrameBuffer::boundBuffer = 0;

void FrameBuffer::release() {
	if (boundBuffer != 0)
		glViewport(0, 0, clientSize.x, clientSize.y);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	boundBuffer = 0;
}

//

FrameBuffer::FrameBuffer(size_t width, size_t height) {
	this->width = width;
	this->height = height;

	init();
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &handle);
}

bool FrameBuffer::isComplete() {
	bind();
	bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	release();

	return complete;
}

void FrameBuffer::init() {
	glGenFramebuffers(1, &handle);
}

void FrameBuffer::bind() {
	//If no framebuffer is bound, save viewport size for restoration later
	if (boundBuffer == 0) {
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		clientSize = ivec2(viewport[2], viewport[3]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glViewport(0, 0, width, height);

	boundBuffer = handle;
}

void FrameBuffer::bindTexture(Texture* texture, GLenum attachment) {
	bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->getHandle(), 0);

	//If this is a color attachment, add it to the color attachment list
	if (attachment >= GL_COLOR_ATTACHMENT0 && attachment <= GL_COLOR_ATTACHMENT15)
		colorAttachments.push_back(attachment);
	
	release();
}

void FrameBuffer::bindRenderBuffer(RenderBuffer* buffer, GLenum attachment) {
	buffer->bindToFramebuffer(this);
}