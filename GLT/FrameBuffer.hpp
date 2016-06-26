#pragma once
#include <gl\glew.h>
#include <vector>
#include "Texture.hpp"
#include "RenderBuffer.hpp"

namespace glt {
	class FrameBuffer {
	public:
		static void release();

		//---------

		FrameBuffer(size_t width, size_t height);
		~FrameBuffer();

		size_t getWidth() { return width; }
		size_t getHeight() { return height; }

		bool isComplete();

		const GLuint getHandle() { return handle; }

		void bindTexture(Texture* texture, GLenum attachment);
		void bindRenderBuffer(RenderBuffer* renderBuffer, GLenum attachment);

		void bind();

	private:
		static glm::ivec2 clientSize;
		static GLuint boundBuffer;

		//----------

		std::vector<GLenum> colorAttachments;

		//----------

		GLuint handle;

		size_t width, height;

		void init();
	};
}