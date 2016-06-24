#include "Texture.hpp"

#include "FrameBuffer.hpp"

using namespace glt;

Texture::Texture() {
	init();
}

Texture::Texture(const char* source) {
	init();
	loadFile(source);
}

Texture::~Texture() {
	dispose();
}

void Texture::dispose() {
	glDeleteTextures(1, &handle);
}

void Texture::init() {
	//Generate
	glGenTextures(1, &handle);
	
	bind();

	//Set default parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::bindToFramebuffer(FrameBuffer* framebuffer) {
	//Set stats
	width = framebuffer->getWidth();
	height = framebuffer->getHeight();

	//Bind to null
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	framebuffer->bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);

	//Release the framebuffer
	FrameBuffer::release();
}

void Texture::loadFile(const char* source) {
	//Load image
	unsigned char* image = SOIL_load_image(source, &width, &height, 0, SOIL_LOAD_RGBA);

	//Load into openGL
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	//Free data
	SOIL_free_image_data(image);
}