#pragma once
#include <glm\matrix.hpp>
#include <glm\vec3.hpp>

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
		float aspectRatio = 800.f / 600.f;

		bool isPerspective = true;
		bool useLookAt = true;

	private:
		mat4 matrix;
	};
}