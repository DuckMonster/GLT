#include "Camera.hpp"
#include <glm\gtc\matrix_transform.hpp>
using namespace glt;

mat4 Camera::getMatrix() {
	mat4 projection = glm::perspective(fieldOfView, aspectRatio, near, far);

	mat4 view(1.0f);
	view = rotate(view, rotation.z, vec3(0, 0, 1));
	view = rotate(view, rotation.y, vec3(1, 0, 1));
	view = rotate(view, rotation.x, vec3(0, 1, 0));
	view = translate(view, -position);

	return projection * view;
}