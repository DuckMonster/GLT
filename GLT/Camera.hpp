#pragma once
#include <glm\glm.hpp>
#include "Ray.hpp"

using namespace glm;

namespace glt {
	class Camera {
	public:
		mat4 getMatrix();
		
		vec3 position = vec3(0.f, 0.f, 1.f);
		vec3 target = vec3(0.f);
		vec3 up = vec3(0.f, 1.f, 0.f);

		vec3 rotation = vec3(0.f);

		float fieldOfView = 0.45f;
		float near = 0.1f;
		float far = 100.f;

		bool perspective = true;
		vec2 screenSize = vec2(800.f, 600.f);

		Camera(vec2 screenSize) { this->screenSize = screenSize; }

		Ray screenToWorld(int x, int y) { return screenToWorld(vec2(x, y)); }
		Ray screenToWorld(vec2 screen);
		vec2 worldToScreen(vec3 world);
		float getAspectRatio() { return screenSize.x / screenSize.y; }

	private:
		mat4 matrix;
	};
}