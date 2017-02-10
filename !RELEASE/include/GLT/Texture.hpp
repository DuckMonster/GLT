#pragma once
#include <SOIL\SOIL.h>
#include <GL\glew.h>
#include <glm\vec2.hpp>

namespace glt {
	class FrameBuffer;

	class Texture {
	public:
		Texture( );
		Texture( const char* source );
		~Texture( );

		void			dispose( );
		void			init( );
		void			init( GLuint internalFormat, GLenum format, GLenum dataType, glm::ivec2 size );

		void			bind( );
		void			bind( unsigned int index );

		glm::ivec2		getSize( ) { return size; }

		void			loadFile( const char* source );

		GLuint			getHandle( ) { return handle; }

	private:
		glm::ivec2		size;

		GLuint			handle = -1;

		//-----------
	};
}