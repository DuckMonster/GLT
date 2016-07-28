#include "Texture.hpp"

#include <sstream>
#include "FrameBuffer.hpp"
#include "Log.hpp"
#include "Time.hpp"

using namespace std;
using namespace glt;
using namespace glt::debug;

Texture::Texture( ) {
	init( );
}

Texture::Texture( const char* source ) {
	init( );
	loadFile( source );
}

Texture::~Texture( ) {
	dispose( );
}

void Texture::dispose( ) {
	if (handle == -1)
		return;

	glDeleteTextures( 1, &handle );

	// Log
	Logger( LOG_TEXTURE_DISPOSE ) << "Texture " << handle << " disposed.\n";
	//

	// Reset log
	handle = -1;
}

void Texture::init( ) {
	// If already initialized, back away
	if (handle != -1)
		return;

	// Generate
	glGenTextures( 1, &handle );

	bind( );

	// Set default parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// Log
	Logger( LOG_TEXTURE_INIT ) << "Texture " << handle << " created.\n";
}

void Texture::init( GLuint internalFormat, GLenum format, GLenum dataType, size_t width, size_t height ) {
	init( );
	bind( );

	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, NULL );
	this->width = width;
	this->height = height;
}

void Texture::bind( ) { bind( 0 ); }
void Texture::bind( unsigned int index ) {
	GLenum indexEnum = GL_TEXTURE0;

	switch (index) {
		case 0: indexEnum = GL_TEXTURE0; break;
		case 1: indexEnum = GL_TEXTURE1; break;
		case 2: indexEnum = GL_TEXTURE2; break;
		case 3: indexEnum = GL_TEXTURE3; break;
		case 4: indexEnum = GL_TEXTURE4; break;
		case 5: indexEnum = GL_TEXTURE5; break;
	}

	glActiveTexture( indexEnum );
	glBindTexture( GL_TEXTURE_2D, handle );

	// Log
	Logger( LOG_TEXTURE_BIND ) << "Texture " << handle << " bound to " << index << "\n";
	//
}

void Texture::loadFile( const char* source ) {
	// Log begin load
	Logger log( LOG_TEXTURE_LOADED );
	log << " Loading texture " << source << " ... ";
	//

	// Time mark for timing
	TimeMark timeMark;

	// Load image
	unsigned char* image = SOIL_load_image( source, &width, &height, 0, SOIL_LOAD_RGBA );

	// Loading failed
	if (image == NULL) {
		// Log
		log << "( FAILED )\n";
		//

		return;
	}

	// Load into openGL
	bind( );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );

	// Free data
	SOIL_free_image_data( image );

	// Log time
	log << "( " << timeMark.duration( ) << " s )\n";
}