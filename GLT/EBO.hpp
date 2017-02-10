#pragma once
#include <GL/glew.h>
#include <vector>

namespace glt {
	class EBO {
	public:
		EBO( ) : handle( -1 ) {}

		void init( );
		void dispose( );

		const GLuint getHandle( ) { return handle; }

		void bind( );
		void setData( const std::vector<size_t>& data );
		void setData( const size_t* ptr, size_t size );
		const std::vector<size_t>& getData( ) { return data; }

	private:
		GLuint				handle;
		std::vector<size_t>	data;
	};
}