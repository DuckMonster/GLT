#include "RenderBuffer.hpp"

#include "FrameBuffer.hpp"

using namespace glt;

RenderBuffer::RenderBuffer( ) {
	init( );
}

void RenderBuffer::init( ) {
	glGenRenderbuffers( 1, &handle );
}

void RenderBuffer::bind( ) {
	glBindRenderbuffer( GL_RENDERBUFFER, handle );
}

void RenderBuffer::bindToFramebuffer( FrameBuffer* buffer ) {
	//Set sizes
	glm::ivec2 size = buffer->getSize( );

	width = size.x;
	height = size.y;

	bind( );
	buffer->bind( );

	//Allocate data
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height );
	//Bind
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, handle );
}