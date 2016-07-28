#pragma once
#include <chrono>
typedef std::chrono::high_resolution_clock hrclock;

#define NANOSECONDS_IN_SECOND 1000000000.f;

namespace glt {
	class Time {
	public:
		static void			init( );
		static double		delta( );
		static double		total( );

	private:
		static bool													initialized;
		static std::chrono::time_point<std::chrono::steady_clock>	bufferTime;
		static std::chrono::time_point<std::chrono::steady_clock>	startTime;
	};

	class TimeMark {
	public:
		TimeMark( );
		double				duration( );

	private:
		std::chrono::time_point<std::chrono::steady_clock>			mark;
	};
}