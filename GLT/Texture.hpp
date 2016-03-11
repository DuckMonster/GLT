#pragma once
#include<SOIL\SOIL.h>
#include<GL\glew.h>

namespace glt {
	class Texture {
	public:
		Texture() {};
		Texture(char* source) { load(source); }
		//~Texture() { dispose(); }

		void dispose() { glDeleteTextures(1, &handle); }

		int getWidth() { return width; }
		int getHeight() { return height; }

		void bind() { glBindTexture(GL_TEXTURE_2D, handle); }
		void load(char* source) {
			//Load image
			unsigned char* image = SOIL_load_image(source, &width, &height, 0, SOIL_LOAD_RGB);

			//Load into openGL
			glGenTextures(1, &handle);
			glBindTexture(GL_TEXTURE_2D, handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Free data
			SOIL_free_image_data(image);
		}

		operator GLuint() { return handle; }

	private:
		int width, height;
		GLuint handle;
	};
}