#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"

namespace glt {
	class Sprite {
	public:
		Sprite();
		Sprite(Texture&);
		Sprite(Texture&, Shader&);
		~Sprite();

		Mesh* getMesh() { return &mesh; }

		void setTexture(Texture*);
		void draw();

	private:
		static Shader* DEFAULT_SHADER;
		static void compileDefaultShader();

		Mesh mesh;
		void initMesh();
	};
}