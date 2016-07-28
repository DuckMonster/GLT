#pragma once
#include <string>
#include <set>
#include <map>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Log.hpp"

namespace glt {
	class Shader {
	public:
		static std::string		ReadFile( std::string filename );

		//-------------

		bool					uniformNotFoundWarning = true;

		//-------------

		Shader( ) {};
		~Shader( ) { dispose( ); }

		void					dispose( );

		// Compile same-filename-files (.vert and .frag)
		void					compileFile( std::string fileName );
		void					compileFile( std::string vertex, std::string fragment );
		// Compile with two absolute filenames
		void					compile( std::string, std::string );

		void					use( );

		// Some OpenGL getters
		GLuint					getHandle( ) { return program; }
		GLuint					getUniform( const char* name );
		GLuint					getAttrib( const char* name );

		// Uniform setters
		bool					set( const char*, bool );
		bool					set( const char*, float );
		bool					set( const char*, int );
		bool					set( const char*, glm::vec2 );
		bool					set( const char*, glm::vec3 );
		bool					set( const char*, glm::vec4 );
		bool					set( const char*, glm::mat2 );
		bool					set( const char*, glm::mat3 );
		bool					set( const char*, glm::mat4 );

	private:
		static GLuint			activeProgram;

		//--------

		GLuint					program = -1;

		std::set<std::string>	attributeErrorSet;
		std::set<std::string>	uniformErrorSet;

		//--------

		GLuint					createShader( GLenum, const char*, debug::Logger& log );
	};
}