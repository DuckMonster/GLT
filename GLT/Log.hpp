#pragma once
#include <string>

namespace glt {
	namespace debug {
		class Log {
		public:
			// Set debug level
			static void			setDebugLevel( size_t level );
			// Write to log
			static void			write( std::string message, size_t level = 0 );

		private:
			// Current minimum debug level being displayed
			static size_t		debugLevel;
		};
	}
}