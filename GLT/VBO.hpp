#pragma once
#include <vector>
#include <iostream>
#include <GL\glew.h>

namespace glt {
	class VBO {
	public:
		GLenum						dataUsage	= GL_STATIC_DRAW;

		VBO( ) { init( 3 ); }
		VBO( size_t size ) { init( size ); }
		~VBO( ) { dispose( ); }

		// Initialize, getting handle from OpenGL happens here
		void						init( size_t );
		void						dispose( );

		void						bind( );

		void						setData( const std::vector<float> );
		void						setData( const float*, size_t );

		const std::vector<float>	getData( );

		void						setDataSize( size_t size ) { this->dataSize = size; }
		size_t						getDataSize( ) { return dataSize; }

		// Setting (or padding) the vertex count
		void						setVertexCount( size_t, float );
		size_t						getVertexCount( );

		GLuint						getHandle( ) const { return handle; }

	private:
		// Singleton for the currently bound VBO
		static GLuint				current;

		// --------

		GLuint						handle		= -1;		// OpenGL handle
		size_t						dataSize	= 3;		// GLSL data size, IE amount of floats per variable (vec2, vec3 etc.)

		std::vector<float>			data;
	};
}