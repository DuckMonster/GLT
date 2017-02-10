#include "FrameBufferUtils.hpp"

using namespace glt;
using namespace glm;

const char* FBU_VERT =
"#version 330 core\n"

"in vec2 a_vertex;"
"in vec2 a_uv;"

"out vec2 f_uv;"

"void main() {"
"	gl_Position = vec4(a_vertex, 0.0, 1.0);"
"	f_uv = a_uv;"
"}";

const char* FBU_FRAG =
"#version 330 core\n"

"uniform sampler2D u_sampler;"

"in vec2 f_uv;"

"out vec4 outColor;"

"void main() {"
"	outColor = texture(u_sampler, f_uv);"
"}";

Mesh*		FrameBufferUtils::displayMesh = nullptr;

Shader& glt::FrameBufferUtils::GetQuadShader( ) {
	static Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compile( FBU_VERT, FBU_FRAG );
		COMPILED = true;
	}

	return SHADER;
}

void FrameBufferUtils::InitQuad( ) {
	if (displayMesh != nullptr)
		return;

	displayMesh = new Mesh( );
	displayMesh->getVertexVBO( )->setDataSize( 2 );

	float quad[]{
		-1.f, -1.f,
		1.f, -1.f,
		-1.f, 1.f,

		1.f, -1.f,
		-1.f, 1.f,
		1.f, 1.f
	};

	float uv[]{
		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 0.f,
		0.f, 1.f,
		1.f, 1.f
	};

	displayMesh->setVertices( quad, sizeof( quad ) );
	displayMesh->setUVS( uv, sizeof( uv ) );
}

void FrameBufferUtils::Quad( ) { Quad( GetQuadShader( ), vec2( -1.f ), vec2( 1.f ) ); }
void FrameBufferUtils::Quad( vec2 min, vec2 max ) { Quad( GetQuadShader( ), min, max ); }
void FrameBufferUtils::Quad( Shader& shader ) { Quad( shader, vec2( -1.f ), vec2( 1.f ) ); }

void FrameBufferUtils::Quad( Shader& shader, vec2 min, vec2 max ) {
	InitQuad( );

	//Set vertices
	float vertices[]{
		min.x, min.y,
		max.x, min.y,
		min.x, max.y,

		max.x, min.y,
		min.x, max.y,
		max.x, max.y
	};

	displayMesh->setVertices( vertices, sizeof( vertices ) );
	displayMesh->bindAttributes( shader, "a_vertex", "", "a_uv", "" );

	shader.use( );

	displayMesh->draw( );
}