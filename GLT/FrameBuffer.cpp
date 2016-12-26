#include "FrameBuffer.hpp"

#include "Log.hpp"

using namespace glt;
using namespace glm;
using namespace glt::debug;

//Static members

ivec2		FrameBuffer::clientSize = vec2( 0 );
GLuint		FrameBuffer::boundBuffer = 0;

void FrameBuffer::release( ) {
	if (boundBuffer != 0) {
		glViewport( 0, 0, clientSize.x, clientSize.y );
		glDrawBuffer( GL_FRONT );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	boundBuffer = 0;
}

//

FrameBuffer::FrameBuffer( glm::ivec2 size ) :
	handle( -1 ), size( size ) {

	init( );
}

FrameBuffer::~FrameBuffer( ) {
	glDeleteFramebuffers( 1, &handle );

	Logger( LOG_FRAMEBUFFER_DISPOSE ) << "Framebuffer " << handle << " disposed.\n";
}

bool FrameBuffer::isComplete( ) {
	bind( );
	bool complete = glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;
	release( );

	return complete;
}

void FrameBuffer::init( ) {
	glGenFramebuffers( 1, &handle );

	Logger( LOG_FRAMEBUFFER_INIT ) << "Framebuffer " << handle << " created.\n";
}

void FrameBuffer::bind( ) {
	//If no framebuffer is bound, save viewport size for restoration later
	if (boundBuffer == 0) {
		int viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );

		clientSize = ivec2( viewport[2], viewport[3] );
	}

	boundBuffer = handle;

	glBindFramebuffer( GL_FRAMEBUFFER, handle );
	glViewport( 0, 0, size.x, size.y );

	//Bind color buffers
	glDrawBuffers( colorAttachments.size( ), colorAttachments.size( ) > 0 ? &colorAttachments[0] : NULL );

	// Log
	Logger( LOG_FRAMEBUFFER_BIND ) << "Framebuffer " << handle << " bound.\n";
}

void FrameBuffer::bindTexture( Texture& texture, GLenum attachment ) {
	bind( );

	// Log
	Logger log( LOG_FRAMEBUFFER_TEXTURE );
	log << "Framebuffer " << handle << " ";

	glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getHandle( ), 0 );

	// If this is a color attachment, add it to the color attachment list
	if (attachment >= GL_COLOR_ATTACHMENT0 && attachment <= GL_COLOR_ATTACHMENT15) {
		colorAttachments.push_back( attachment );

		log << "color attachment " << attachment - GL_COLOR_ATTACHMENT0 << " bound. ( " << colorAttachments.size( ) << " )\n";
	}
	else if (attachment == GL_DEPTH_ATTACHMENT)
		log << "depth attachment.\n";
	else if (attachment == GL_STENCIL_ATTACHMENT)
		log << "stencil attachment.\n";
	else if (attachment == GL_DEPTH_STENCIL_ATTACHMENT)
		log << "depth/stencil attachment.\n";

	release( );
}

void FrameBuffer::bindRenderBuffer( RenderBuffer& buffer, GLenum attachment ) {
	buffer.bindToFramebuffer( this );
}