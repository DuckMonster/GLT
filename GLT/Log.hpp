#pragma once
#include <string>

namespace glt {
	namespace debug {
		const size_t		LOG_VBO_INIT = 1;
		const size_t		LOG_VBO_DISPOSE = 1;
		const size_t		LOG_VBO_BIND = 0;
		const size_t		LOG_VBO_DATA = 0;

		const size_t		LOG_VAO_INIT = 1;
		const size_t		LOG_VAO_DISPOSE = 1;
		const size_t		LOG_VAO_BIND = 0;
		const size_t		LOG_VAO_ATTR = 1;

		const size_t		LOG_SHADER_DISPOSE = 1;
		const size_t		LOG_SHADER_COMPILE = 2;
		const size_t		LOG_SHADER_BIND = 0;
		const size_t		LOG_SHADER_ERROR = 3;

		const size_t		LOG_MESH_INIT = 1;
		const size_t		LOG_MESH_DISPOSE = 1;

		const size_t		LOG_TEXTURE_INIT = 1;
		const size_t		LOG_TEXTURE_DISPOSE = 1;
		const size_t		LOG_TEXTURE_BIND = 0;

		const size_t		LOG_FRAMEBUFFER_INIT = 1;
		const size_t		LOG_FRAMEBUFFER_DISPOSE = 1;
		const size_t		LOG_FRAMEBUFFER_BIND = 0;
		const size_t		LOG_FRAMEBUFFER_TEXTURE = 1;

		const size_t		LOG_MESH_LOADED = 3;
		const size_t		LOG_TEXTURE_LOADED = 3;

		class Logger {
		public:
			Logger( size_t level );

			Logger& operator<<( char* val );
			Logger& operator<<( std::string val );
			Logger& operator<<( int val );
			Logger& operator<<( size_t val );
			Logger& operator<<( float val );
			Logger& operator<<( double val );

		private:
			// Log level this logger is currently working with
			size_t					level;
		};
	}
}