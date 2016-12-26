#pragma once
#include <GL\glew.h>
#include <iostream>
#include "VBO.hpp"

namespace glt {
	class VAO {
	public:
		VAO() { init(); }
		~VAO() { dispose(); }

		void				dispose( );
		void				bind( );

		template<typename T>
		void				bindBufferToAttr(VBO<T>* vbo, GLuint attribute);
		void				bindBufferToAttr(GLuint vbo, GLuint size, GLuint attribute);

		const GLuint		getHandle( ) { return handle; }

	private:
		// Current bound VAO

		static GLuint		current;

		// --------

		GLuint				handle		= -1;

		void				init( );
	};
}

template<typename T>
void VAO::bindBufferToAttr( VBO<T>* vbo, GLuint attribute ) {
	bindBufferToAttr( vbo->getHandle( ), vbo->getDataSize( ), attribute );
}