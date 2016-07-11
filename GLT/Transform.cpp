#include "Transform.hpp"

using namespace glm;
using namespace glt;

Transform::Transform( ) {
	position			= vec3( 0.f );
	rotation			= vec3( 0.f );
	scale				= vec3( 1.f );
}

Transform::Transform( vec3 position, vec3 rotation, vec3 scale ) {
	this->position		= position;
	this->rotation		= rotation;
	this->scale			= scale;
}

mat4 Transform::getMatrix( ) {
	// Return the buffer if the transform isn't dirty
	if (!isDirty( ))
		return matrixBuffer;

	//// ROTATION
	float xc		= cos( radians( rotation.x ) ); // cos of x
	float xs		= sin( radians( rotation.x ) ); // sin of x

	float yc		= cos( radians( rotation.y ) ); // cos of y
	float ys		= sin( radians( rotation.y ) ); // sin of y

	float zc		= cos( radians( rotation.z ) ); // cos of z
	float zs		= sin( radians( rotation.z ) ); // sin of z

	//// SCALE
	float sx		= scale.x;
	float sy		= scale.y;
	float sz		= scale.z;

	//// TRANSLATION
	float tx		= position.x;
	float ty		= position.y;
	float tz		= position.z;

	// Headache-inducing formula for Translation * Rotation y * Rotation x * Rotation z * Scale
	// Looking at it makes my skin crawl

	// Pre-transposed
	mat4 model(
		yc*zc*sx + xs*ys*zs*sx,		xc*zs*sx,		-ys*zc*sx + xs*yc*zs*sx,		0,
		xs*ys*zc*sy - yc*zs*sy,		xc*zc*sy,		xs*yc*zc*sy + ys*zs*sy,			0,
		xc*ys*sz,					-xs*sz,			xc*yc*sz,						0,
		tx,							ty,				tz,								1
		);

	// Transpose it for glm :)
	// modelMatrix = transpose(model);
	matrixBuffer = model; // <--- pre-transposed for performance

	// Now that the matrix has been generated, mark it as clean
	clean( );

	return matrixBuffer;
}

bool Transform::isDirty( ) {
	return		position	!= position_old ||
				rotation	!= rotation_old ||
				scale		!= scale_old;
}

void Transform::clean( ) {
	position_old	= position;
	rotation_old	= rotation;
	scale_old		= scale;
}