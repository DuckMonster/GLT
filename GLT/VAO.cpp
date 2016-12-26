#include "VAO.hpp"

#include "VBO.hpp"
#include "Log.hpp"

using namespace glt;
using namespace glt::debug;

// Current bounded VAO
GLuint		VAO::current	= -1;

void VAO::init( ) {
	// Already initialized
	if (handle != -1)
		return;

	glGenVertexArrays( 1, &handle );
	Logger( LOG_VAO_INIT ) << "VAO " << handle << " created.\n";
}

void VAO::dispose( ) {
	// Not mapped to a VBO
	if (handle == -1)
		return;

	glDeleteVertexArrays( 1, &handle );

	// Log
	Logger( LOG_VAO_DISPOSE ) << "VAO " << handle << " disposed.\n";

	handle = -1;
}

void VAO::bind( ) {
	// Bind
	glBindVertexArray( handle );
	current			= handle;	// Update current VAO

	// Log
	Logger( LOG_VAO_BIND ) << "VAO " << handle << " bound.\n";
}

void VAO::bindBufferToAttr(GLuint vbo, GLuint size, GLuint attribute) {
	bind();
	glEnableVertexAttribArray(attribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute, size, GL_FLOAT, false, 0, 0);

	// Log
	Logger( LOG_VAO_ATTR ) << "VBO " << vbo << " bound to attribute " << attribute << ".\n";
}