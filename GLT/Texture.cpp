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
	if (handle == -1)
		return;

	glDeleteTextures(1, &handle);
	handle = -1;
}

void Texture::init() {
	//If already initialized, back away
	if (handle != -1)
		return;

	//Generate
	glGenTextures(1, &handle);

	bind();

	//Set default parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::init(GLuint internalFormat, GLenum format, GLenum dataType, size_t width, size_t height) {
	init();
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, NULL);
	this->width = width;
	this->height = height;
}

void Texture::bind() { bind(0); }
void Texture::bind(unsigned int index) {
	GLenum indexEnum = GL_TEXTURE0;

	switch (index) {
	case 0: indexEnum = GL_TEXTURE0; break;
	case 1: indexEnum = GL_TEXTURE1; break;
	case 2: indexEnum = GL_TEXTURE2; break;
	case 3: indexEnum = GL_TEXTURE3; break;
	case 4: indexEnum = GL_TEXTURE4; break;
	case 5: indexEnum = GL_TEXTURE5; break;
	}

	glActiveTexture(indexEnum);
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::loadFile(const char* source) {
	//Load image
	unsigned char* image = SOIL_load_image(source, &width, &height, 0, SOIL_LOAD_RGBA);

	if (image == NULL) {
		std::cout << "Couldn't load texture " << source << "!\n";
		return;
	}

	//Load into openGL
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	//Free data
	SOIL_free_image_data(image);
}