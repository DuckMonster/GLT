#include "EBO.hpp"
#include "Log.hpp"

using namespace glt;
using namespace glt::debug;

void glt::EBO::init( ) {
	if (handle != -1)
		return;

	glGenBuffers( 1, &handle );

	Logger( LOG_EBO_INIT ) << "EBO " << handle << " created.\n";
}

void glt::EBO::dispose( ) {
	if (handle == -1)
		return;

	glDeleteBuffers( 1, &handle );
	Logger( LOG_EBO_DISPOSE ) << "EBO " << handle << " disposed.\n";

	handle = -1;
}

void glt::EBO::bind( ) {
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
	Logger( LOG_EBO_BIND ) << "EBO " << handle << " disposed.\n";
}

void glt::EBO::setData( const std::vector<size_t>& data ) {
	setData( &data[0], sizeof( size_t ) * data.size( ) );
}

void glt::EBO::setData( const size_t * ptr, size_t size ) {
	bind( );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW );
	data.assign( ptr, ptr + (size / sizeof( ptr )) );
	Logger( LOG_EBO_DATA ) << "EBO " << handle << " data ( " << size << " )\n";
}
