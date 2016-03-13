#include "Camera.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace glt;

Camera* Camera::active = nullptr;

Camera::Camera(vec2 screenSize) {
	if (active == nullptr)
		setActive();

	this->screenSize = screenSize;
}

mat4 Camera::getMatrix() {
	mat4 projection = glm::perspective(fieldOfView, getAspectRatio(), near, far);

	mat4 view(1.0f);
	view = rotate(view, radians(rotation.z), vec3(0, 0, 1));
	view = rotate(view, radians(rotation.x), vec3(1, 0, 0));
	view = rotate(view, radians(rotation.y), vec3(0, 1, 0));
	view = translate(view, -position);

	return projection * view;
}

Ray Camera::screenToWorld(vec2 screen) {
	screen = clamp(screen, vec2(0.f), screenSize);
	screen = (screen / screenSize - vec2(0.5f)) * vec2(2.f, -2.f);

	mat4 projView = getMatrix();
	projView = inverse(projView);

	vec4 world = projView * vec4(screen.x, screen.y, -1, 1);
	world = world / world.w;

	return Ray::fromTo(position, vec3(world));
}

vec2 Camera::worldToScreen(vec3 world) {
	return vec2(getMatrix() * vec4(world, 1.0));
}

void Camera::setActive() {
	Camera::active = this;
}

void Camera::updateShader(GLuint program, const char* vpName) {
	glUniformMatrix4fv(glGetUniformLocation(program, vpName), 1, false, value_ptr(getMatrix()));
}