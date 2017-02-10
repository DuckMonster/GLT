#pragma once
#include <vector>
#include <iostream>
#include <GL\glew.h>
#include "Log.hpp"

namespace glt {
	template<typename T>
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

		void						setData( const std::vector<T>& );
		void						setData( const T*, size_t );

		const std::vector<float>	getData( );

		void						setDataSize( size_t size ) { this->dataSize = size; }
		size_t						getDataSize( ) { return dataSize; }

		// Setting (or padding) the vertex count
		void						setVertexCount( size_t, const T& );
		size_t						getVertexCount( );

		GLuint						getHandle( ) const { return handle; }

	private:
		// Singleton for the currently bound VBO
		static GLuint				current;

		// --------

		GLuint						handle		= -1;		// OpenGL handle
		size_t						dataSize	= 3;		// GLSL data size, IE amount of floats per variable (vec2, vec3 etc.)

		std::vector<T>				data;
	};
}

using namespace glt;
using namespace glt::debug;

// Currently bound VBO
template<typename T>
GLuint VBO<T>::current = -1;

template<typename T>
void VBO<T>::init( size_t size ) {
	// Check if already initialized
	if (handle != -1)
		return;

	setDataSize( size );

	// Gen the buffer
	glGenBuffers( 1, &handle );

	// Log
	Logger( LOG_VBO_INIT ) << "VBO " << handle << " created.\n";
}

template<typename T>
void VBO<T>::dispose( ) {
	// Check if it's already initialized
	if (handle == -1)
		return;

	glDeleteBuffers( 1, &handle );

	// Log
	Logger( LOG_VBO_DISPOSE ) << "VBO " << handle << " disposed.\n";

	// Reset the handle
	handle = -1;
}

template<typename T>
void VBO<T>::bind( ) {
	// Check if not initialized
	if (handle == -1)
		throw std::exception( "VBO not initialized" );

	// Bind
	glBindBuffer( GL_ARRAY_BUFFER, handle );
	current		= handle;		// Update current VBO

								// Log
	Logger( LOG_VBO_BIND ) << "VBO " << handle << " bound.\n";
}

template<typename T>
void VBO<T>::setData( const std::vector<T>& data ) {
	setData( &data[0], data.size( ) * sizeof( T ) );
}

template<typename T>
void VBO<T>::setData( const T* data, size_t size ) {
	bind( );

	// Upload to OpenGL
	glBufferData( GL_ARRAY_BUFFER, size, data, dataUsage );

	// Assign data to vector
	this->data.assign( data, data + size / sizeof( T ) );

	// Log
	Logger( LOG_VBO_DATA ) << "VBO " << handle << " data ( " << size << " )\n";
}

template<typename T>
const std::vector<float> VBO<T>::getData( ) {
	return data;
}

template<typename T>
void VBO<T>::setVertexCount( size_t count, const T& defValue ) {
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
		setData( &data[0], dataCount * sizeof( T ) );
}

template<typename T>
size_t VBO<T>::getVertexCount( ) {
	return data.size( ) / dataSize;
}