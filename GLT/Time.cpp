#include "Time.hpp"
using namespace glt;

bool Time::initialized = false;
std::chrono::time_point<std::chrono::steady_clock> Time::bufferTime;
std::chrono::time_point<std::chrono::steady_clock> Time::startTime;

void Time::init() {
	Time::startTime = Time::bufferTime = hrclock::now();
	initialized = true;
}

double Time::getDelta() {
	if (!initialized)
		init();

	auto newTime = hrclock::now();
	double delta = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - Time::bufferTime).count() / 1000000000.0;
	Time::bufferTime = newTime;

	return delta;
}

double Time::total() {
	if (!initialized)
		init();

	auto newTime = hrclock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - Time::startTime).count() / 1000000000.0;
}