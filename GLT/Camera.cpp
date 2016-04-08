#include "Camera.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

using namespace glt;

Camera* Camera::active = nullptr;

Camera::Camera(vec2 screenSize) {
	if (active == nullptr)
		setActive();

	this->screenSize = screenSize;
}

void Camera::checkDirty() {
	if (position_old != position ||
		direction_old != direction ||
		fov_old != fieldOfView ||
		near_old != near ||
		far_old != far ||
		perspective_old != perspective ||
		screenSize_old != screenSize) {
		isDirty = true;
	}
}

void Camera::clean() {
	position_old = position;
	direction_old = direction;

	fov_old = fieldOfView;
	near_old = near;
	far_old = far;

	perspective_old = perspective;
	screenSize_old = screenSize;

	isDirty = false;
}

mat4 Camera::getMatrix() {
	checkDirty();

	if (!isDirty)
		return cameraMatrix;

	clean();

	mat4 projection = glm::perspective(fieldOfView, getAspectRatio(), near, far);
	mat4 view = glm::lookAt(position, position + direction, vec3(0.f, 1.f, 0.f));

	cameraMatrix = projection * view;
	return cameraMatrix;
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