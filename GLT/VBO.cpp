#include "VBO.hpp"

#include "Log.hpp"

using namespace glt;
using namespace glt::debug;

// Currently bound VBO
GLuint VBO::current = -1;

void VBO::init( size_t size ) {
	// Check if already initialized
	if (handle != -1)
		return;

	setDataSize( size );

	// Gen the buffer
	glGenBuffers( 1, &handle );

	// Log
	Logger( LOG_VBO_INIT ) << "VBO " << handle << " created.\n";
}

void VBO::dispose( ) {
	// Check if it's already initialized
	if (handle == -1)
		return;

	glDeleteBuffers( 1, &handle );

	// Log
	Logger( LOG_VBO_DISPOSE ) << "VBO " << handle << " disposed.\n";

	// Reset the handle
	handle = -1;
}

void VBO::bind( ) {
	// Check if not initialized
	if (handle == -1)
		throw std::exception( "VBO not initialized" );

	// VBO already bound
	if ( current == handle )
		return;

	// Bind
	glBindBuffer( GL_ARRAY_BUFFER, handle );
	current		= handle;		// Update current VBO

	// Log
	Logger( LOG_VBO_BIND ) << "VBO " << handle << " bound.\n";
}

void VBO::setData( const std::vector<float> data ) {
	setData( &data[0], data.size( ) * sizeof( float ) );
}

void VBO::setData( const float* data, size_t size ) {
	bind( );

	// Upload to OpenGL
	glBufferData( GL_ARRAY_BUFFER, size, data, dataUsage );

	// Assign data to vector
	this->data.assign( data, data + size / sizeof( float ) );

	// Log
	Logger( LOG_VBO_DATA ) << "VBO " << handle << " data ( " << size << " )\n";
}

const std::vector<float> VBO::getData( ) {
	return data;
}

void VBO::setVertexCount( size_t count, float defValue ) {
	size_t		dataCount = count * dataSize;

	// Already the right data size
	if (dataCount == data.size( ))
		return;

	// Add more data
	if (dataCount > data.size( )) {
		data.insert( data.end( ), dataCount - data.size( ), defValue );
		setData( data );
	}

	// Remove data
	else
		setData( &data[0], dataCount * sizeof( float ) );
}

size_t VBO::getVertexCount( ) {
	return data.size( ) / dataSize;
}