#include "Ray.hpp"
using namespace glt;
using namespace glm;

Ray Ray::fromTo(vec3 a, vec3 b) {
	return Ray(a, normalize(b - a));
}

Ray::Ray(vec3 start, vec3 direction) {
	this->start = start;
	this->direction = direction;
}

vec3 Ray::overlapZ() {
	return (*this) * (-start.z / direction.z);
}

vec3 Ray::overlapPlane(vec3 point, vec3 normal) {
	float dis = dot(point - start, normal);
	float dir = dot(direction, normal);

	return (*this) * (dis / dir);
}

vec3 Ray::operator*(float f) {
	return start + direction * f;
}