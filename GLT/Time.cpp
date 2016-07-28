#include "Time.hpp"

using namespace glt;
using namespace std::chrono;

bool						Time::initialized = false;
time_point<steady_clock>	Time::bufferTime;
time_point<steady_clock>	Time::startTime;

void Time::init( ) {
	Time::startTime = Time::bufferTime = hrclock::now( );
	initialized = true;
}

double Time::delta( ) {
	if (!initialized)
		init( );

	auto newTime = hrclock::now( );
	double delta = duration_cast<nanoseconds>(newTime - Time::bufferTime).count( ) / NANOSECONDS_IN_SECOND;
	Time::bufferTime = newTime;

	return delta;
}

double Time::total( ) {
	if (!initialized)
		init( );

	auto newTime = hrclock::now( );
	return duration_cast<nanoseconds>(newTime - Time::startTime).count( ) / NANOSECONDS_IN_SECOND;
}

TimeMark::TimeMark( ) {
	mark = hrclock::now( );
}

double TimeMark::duration( ) {
	auto currentTime = hrclock::now( );
	return duration_cast<nanoseconds>(currentTime - mark).count( ) / NANOSECONDS_IN_SECOND;
}