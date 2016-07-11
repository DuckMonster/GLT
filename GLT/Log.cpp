#include "Log.hpp"

#include <iostream>
#include <ctime>

using namespace glt::debug;
using namespace std;

size_t Log::debugLevel = 0;

///<summary>Minimum debug level to output. The higher the level the more important the message.</summary>
void Log::setDebugLevel( size_t level ) {
	debugLevel = level;

	write( "Debug level set to " + debugLevel, -1 );
}

///<symmary>Write message to log</summary>
void Log::write( string message, size_t level ) {
	if (level < debugLevel)
		return;

	// Get the time
	time_t timer;
	time( &timer );

	tm* time;
	time = localtime( &timer );

	// TODO - Add output stream customization support
	cout << "[" << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << "] ";
	cout << message << "\n";
}