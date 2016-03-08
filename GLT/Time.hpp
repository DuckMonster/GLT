#pragma once
#include <chrono>
typedef std::chrono::high_resolution_clock hrclock;

namespace glt {
	class Time {
	public:
		void init() {
			startTime = bufferTime = hrclock::now();
		}

		double getDelta() {
			auto newTime = hrclock::now();
			double delta = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - bufferTime).count() / 1000000000.0;
			bufferTime = newTime;
		}

		double total() {
			auto newTime = hrclock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - bufferTime).count() / 1000000000.0;
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> bufferTime;
		std::chrono::time_point<std::chrono::steady_clock> startTime;
	};
}