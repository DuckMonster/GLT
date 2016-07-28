#include "Log.hpp"

#include <iostream>
#include <ctime>

#include "LogLevel.hpp"

using namespace glt::debug;
using namespace std;

///////
// LOGGER
///////

Logger::Logger( size_t level ) {
	this->level = level;

	// Get the time
	time_t timer;
	time( &timer );

	tm* time;
	time = localtime( &timer );

	(*this) << "[" << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << "] ";
}

Logger& Logger::operator<<( char* val ) {
	// Check debug level
	if (level < logLevel)
		return *this;

	cout << val;
	return *this;
}
Logger& Logger::operator<<( string val ) {
	// Check debug level
	if (level < logLevel)
		return *this;

	cout << val;
	return *this;
}
Logger& Logger::operator<<( int val ) {
	// Check debug level
	if (level < logLevel)
		return *this;

	cout << val;
	return *this;
}
Logger& Logger::operator<<( size_t val ) {
	// Check debug level
	if (level < logLevel)
		return *this;

	cout << val;
	return *this;
}
Logger& Logger::operator<<( float val ) {
	// Check debug level
	if (level < logLevel)
		return *this;

	cout << val;
	return *this;
}
Logger& Logger::operator<<( double val ) {
	// Check debug level
	if (level < logLevel)
		return *this;

	cout << val;
	return *this;
}