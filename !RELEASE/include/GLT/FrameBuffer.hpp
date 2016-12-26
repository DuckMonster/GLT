#pragma once
#include <gl\glew.h>
#include <vector>
#include "Texture.hpp"
#include "RenderBuffer.hpp"

namespace glt {
	class FrameBuffer {
	public:
		static void		release( );

		//---------

		FrameBuffer( glm::ivec2 size );
		~FrameBuffer( );

		glm::ivec2		getSize( ) { return size; }

		bool			isComplete( );

		const GLuint	getHandle( ) { return handle; }

		void			bindTexture( Texture& texture, GLenum attachment );
		void			bindRenderBuffer( RenderBuffer& renderBuffer, GLenum attachment );

		void			bind( );

	private:
		static glm::ivec2		clientSize;
		static GLuint			boundBuffer;

		//----------

		GLuint					handle;
		glm::ivec2				size;

		std::vector<GLenum>		colorAttachments;

		//----------

		void					init( );
	};
}