#pragma once
#include<glm\vec3.hpp>
#include<glm\matrix.hpp>
#include<vector>

#include "VAO.hpp"
#include "VBO.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace glt {
	class Mesh {
	public:
		enum Primitive {
			Quad,
			Triangle
		};

		GLuint				drawMode = GL_TRIANGLES;

		//-----------

		Mesh( );
		Mesh( Primitive primitive );

		void				setVertices( const float*, int );
		void				setUVS( const float*, int );
		void				setColors( const float*, int );
		void				setNormals( const float*, int );

		VAO*				getVAO( ) { return &vao; }
		VBO*				getVertexVBO( ) { return &vbo_vertices; }
		VBO*				getColorVBO( ) { return &vbo_colors; }
		VBO*				getUvVBO( ) { return &vbo_uvs; }
		VBO*				getNormalVBO( ) { return &vbo_normals; }
		size_t				getVertexCount( ) { return vertexCount; }

		// Bind all the VBO's to attributes
		void				bindAttributes( Shader& shader, const char* vertex, const char* normal, const char* uv, const char* color );
		void				bindAttributes( GLuint vertex, GLuint normal, GLuint uv, GLuint color );

		void				setDataUsage( GLenum usage );

		void				flipUV( );		// Flip the Y value of all UV's

		void				loadPrimitive( Primitive primitive );

		void				draw( );

	private:
		VAO					vao;
		VBO					vbo_vertices;
		VBO					vbo_colors;
		VBO					vbo_uvs;
		VBO					vbo_normals;

		std::size_t			vertexCount = 0;

		//-----------

		void				init( );
		void				fillVertices( size_t );
	};
}