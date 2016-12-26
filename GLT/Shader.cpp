#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm\gtc\type_ptr.hpp>

#include "Log.hpp"

using namespace std;
using namespace glt;
using namespace glm;
using namespace glt::debug;

// Currently used program
GLuint Shader::activeProgram = -1;

// Read file to string
string Shader::ReadFile( std::string filename ) {
	// File stream
	ifstream	file( filename );

	// Source buffer
	string		source = "";

	if (file.is_open( )) {
		// Read a line at a time
		string	line = "";

		while (getline( file, line )) {
			// Append to buffer
			source += line + "\n";
		}

		file.close( );
	}

	return source;
}

void Shader::dispose( ) {
	if (program == -1)
		return;

	glDeleteProgram( program );

	// Log
	Logger( LOG_SHADER_DISPOSE ) << "Shader " << program << " disposed.\n";

	// And reset handle
	program = -1;
}

// Compile .vert and .frag file pair
void Shader::compileFile( string srcFileName ) {
	// Log
	Logger( LOG_SHADER_COMPILE ) << "Compiling shader [ " << srcFileName << " ]\n";

	compile( ReadFile( srcFileName + ".vert" ), ReadFile( srcFileName + ".frag" ) );
}

void Shader::compileFile( string vertex, string fragment ) {
	// Log
	Logger( LOG_SHADER_COMPILE ) << "Compiling shader [ " << vertex << " ] [ " << fragment << " ]\n";

	compile( ReadFile( vertex ), ReadFile( fragment ) );
}

// Compile two 
void Shader::compile( string vertexSrc, string fragmentSrc ) {
	// Dispose shader in case it already compiled something else
	dispose( );

	Logger log( LOG_SHADER_COMPILE );

	// Init
	program = glCreateProgram( );
	log << "Shader " << program << " created.\n";

	GLuint vertex = createShader( GL_VERTEX_SHADER, vertexSrc.c_str( ), log );
	GLuint fragment = createShader( GL_FRAGMENT_SHADER, fragmentSrc.c_str( ), log );

	// Link program
	glAttachShader( program, vertex );
	glAttachShader( program, fragment );
	glLinkProgram( program );

	// Cleanup shaders after linking
	glDetachShader( program, vertex );
	glDetachShader( program, fragment );
	glDeleteShader( vertex );
	glDeleteShader( fragment );
}

GLuint Shader::createShader( GLenum type, const char* src, Logger& log ) {
	// Init and compile
	GLuint shader = glCreateShader( type );
	glShaderSource( shader, 1, &src, 0 );
	glCompileShader( shader );

	// Error check
	char buffer[1024];
	glGetShaderInfoLog( shader, 1024, nullptr, buffer );

	// ... in case of error, print it to console
	switch (type) {
		case GL_VERTEX_SHADER:
			log << "[VERTEX]\n";
			break;

		case GL_FRAGMENT_SHADER:
			log << "[FRAGMENT]\n";
			break;

		case GL_GEOMETRY_SHADER:
			log << "[GEOMETRY]\n";
			break;
	}

	log << buffer;

	return shader;
}

void Shader::use( ) {
	// This shader is already used
	if (program == activeProgram)
		return;

	glUseProgram( program );
	activeProgram = program;

	// Log
	Logger( LOG_SHADER_BIND ) << "Shader " << program << " used.\n";
}

GLuint Shader::getUniform( const char* name ) {
	GLuint		uniformPos = glGetUniformLocation( program, name );

	//The uniform doesn't exist, but hasn't been tried before
	if (uniformPos == -1 && uniformNotFoundWarning && uniformErrorSet.find( name ) == uniformErrorSet.end( )) {
		uniformErrorSet.insert( name );

		// Log the error
		Logger( LOG_SHADER_ERROR ) << "Couldn't find uniform " << name << "!\n";
		//
	}

	return uniformPos;
}

GLuint Shader::getAttrib( const char* name ) {
	GLuint		attribPos = glGetAttribLocation( program, name );

	if (attribPos == -1 && attributeErrorSet.find( name ) == attributeErrorSet.end( )) {
		attributeErrorSet.insert( name );

		// Log the error
		Logger( LOG_SHADER_ERROR ) << "Couldn't find attribute " << name << "!\n";
		//
	}

	return attribPos;
}

// Uniform sets
// BOOL
bool Shader::set( const char* name, bool value ) { return set( name, value ? 1 : 0 ); }

// FLOAT
bool Shader::set( const char* name, float value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform1f( uniformLocation, value );
	return true;
}

// INTEGER
bool Shader::set( const char* name, int value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform1i( uniformLocation, value );
	return true;
}

// VECTORS
bool Shader::set( const char* name, vec2 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform2f( uniformLocation, value.x, value.y );
	return true;
}
bool Shader::set( const char* name, vec3 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform3f( uniformLocation, value.x, value.y, value.z );
	return true;
}
bool Shader::set( const char* name, vec4 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform4f( uniformLocation, value.x, value.y, value.z, value.w );
	return true;
}
bool Shader::set( const char* name, ivec2 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform2iv( uniformLocation, 1, glm::value_ptr( value ) );
	return true;
}
bool Shader::set( const char* name, ivec3 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform3iv( uniformLocation, 1, glm::value_ptr( value ) );
	return true;
}
bool Shader::set( const char* name, ivec4 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform4iv( uniformLocation, 1, glm::value_ptr( value ) );
	return true;
}

// MATRICES
bool Shader::set( const char* name, mat2 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniformMatrix2fv( uniformLocation, 1, false, value_ptr( value ) );
	return true;
}
bool Shader::set( const char* name, mat3 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniformMatrix3fv( uniformLocation, 1, false, value_ptr( value ) );
	return true;
}
bool Shader::set( const char* name, mat4 value ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniformMatrix4fv( uniformLocation, 1, false, value_ptr( value ) );
	return true;
}

bool glt::Shader::set( const char* name, float* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform1fv( uniformLocation, count, (GLfloat*)value );
	return true;
}
bool glt::Shader::set( const char* name, int* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform1iv( uniformLocation, count, (GLint*)value );
	return true;
}
bool glt::Shader::set( const char* name, vec2* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform2fv( uniformLocation, count, (GLfloat*)value );
	return true;
}
bool glt::Shader::set( const char* name, vec3* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform3fv( uniformLocation, count, (GLfloat*)value );
	return true;
}
bool glt::Shader::set( const char* name, vec4* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform4fv( uniformLocation, count, (GLfloat*)value );
	return true;
}
bool glt::Shader::set( const char* name, ivec2* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform2iv( uniformLocation, count, (GLint*)value );
	return true;
}
bool glt::Shader::set( const char* name, ivec3* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform3iv( uniformLocation, count, (GLint*)value );
	return true;
}
bool glt::Shader::set( const char* name, ivec4* value, size_t count ) {
	GLuint		uniformLocation = getUniform( name );

	// Error checking
	if (uniformLocation == -1)
		return false;

	use( );
	glUniform4iv( uniformLocation, count, (GLint*)value );
	return true;
}
