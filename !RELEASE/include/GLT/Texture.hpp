#pragma once
#include<SOIL\SOIL.h>
#include<GL\glew.h>

namespace glt {
	class FrameBuffer;

	class Texture {
	public:
		Texture();
		Texture(const char* source);
		~Texture();

		void			dispose();
		void			init();
		void			init(GLuint internalFormat, GLenum format, GLenum dataType, size_t width, size_t height);

		void			bind( );
		void			bind( unsigned int index );

		int				getWidth() { return width; }
		int				getHeight() { return height; }

		void			loadFile(const char* source);

		GLuint			getHandle() { return handle; }

	private:
		int				width = -1;
		int				height = -1;

		GLuint			handle = -1;

		//-----------
	};
}