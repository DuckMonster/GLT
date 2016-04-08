#include "MeshDrawer.hpp"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Camera.hpp"

using namespace glm;
using namespace glt;

#pragma region
const char* SRC_VERTEX =
"#version 450\n"
"in vec3 v_position;"
"in vec4 v_color;"
"in vec2 v_uv;"

"uniform mat4 u_camera;"
"uniform mat4 u_model;"

"out vec4 f_color;"
"out vec2 f_uv;"

"void main() {"
"	gl_Position = u_camera * u_model * vec4(v_position, 1.0);"
"	f_color = v_color;"
"	f_uv = v_uv;"
"}";

const char* SRC_FRAGMENT =
"#version 450\n"
"uniform sampler2D u_texture;"
"uniform bool u_usingTexture;"

"in vec4 f_color;"
"in vec2 f_uv;"

"uniform vec4 u_blendColor;"

"out vec4 outColor;"

"void main() {"
"	vec4 color = vec4(1.0);"

"	if (u_usingTexture) {"
"		color = color * texture(u_texture, f_uv);"
"	}"

"	outColor = color * f_color * u_blendColor;"
"}";
#pragma endregion Default shader source

Shader* MeshDrawer::DEFAULT_SHADER = nullptr;
void MeshDrawer::compileDefaultShader() {
	if (MeshDrawer::DEFAULT_SHADER != nullptr)
		return;

	MeshDrawer::DEFAULT_SHADER = new Shader(SRC_VERTEX, SRC_FRAGMENT);
}

MeshDrawer::MeshDrawer() {
	compileDefaultShader();
	setShader(DEFAULT_SHADER);
}

MeshDrawer::MeshDrawer(MeshDrawer* copy) {
	position = copy->position;
	scale = copy->scale;
	rotation = copy->rotation;

	modelMatrix = copy->modelMatrix;
	autoGenMatrix = copy->autoGenMatrix;

	clean();

	color = copy->color;

	setShader(copy->shader);
	setMesh(copy->mesh);
}

MeshDrawer::MeshDrawer(Mesh* mesh) {
	compileDefaultShader();
	setShader(DEFAULT_SHADER);
	setMesh(mesh);
}

void MeshDrawer::setMesh(Mesh* mesh) {
	this->mesh = mesh;
	bindAttributes();
}

void MeshDrawer::setShader(Shader* shader) {
	this->shader = shader;
	bindAttributes();
}

void MeshDrawer::bindAttributes() {
	if (mesh == nullptr)
		return;

	VAO* vao = mesh->getVAO();
	vao->bindBufferToAttr(mesh->getVertexVBO(), shader->getAttrib("v_position"));
	vao->bindBufferToAttr(mesh->getColorVBO(), shader->getAttrib("v_color"));
	vao->bindBufferToAttr(mesh->getUvVBO(), shader->getAttrib("v_uv"));
}

//Transformations
void MeshDrawer::genModelMatrix() {
	checkDirty();

	if (!isDirty)
		return;

	////ROTATION
	//x
	float xc = cos(radians(rotation.x)); //cos of x
	float xs = sin(radians(rotation.x)); //sin of x

	float yc = cos(radians(rotation.y)); //cos of y
	float ys = sin(radians(rotation.y)); //sin of y

	float zc = cos(radians(rotation.z)); //cos of z
	float zs = sin(radians(rotation.z)); //sin of z

	////SCALE
	float sx = scale.x;
	float sy = scale.y;
	float sz = scale.z;

	////TRANSLATION
	float tx = position.x;
	float ty = position.y;
	float tz = position.z;

	//Headache-inducing formula for Translation * Rotation y * Rotation x * Rotation z * Scale
	//If I have to come back here I don't know what to do

	//NOT TRANSPOSED
	/*mat4 model(
	yc*zc*sx + xs*ys*zs*sx, xs*ys*zc*sy - yc*zs*sy, xc*ys*sz, tx,
	xc*zs*sx, xc*zc*sy, -xs*sz, ty,
	-ys*zc*sx + xs*yc*zs*sx, xs*yc*zc*sy + ys*zs*sy, xc*yc*sz, tz,
	0, 0, 0, 1
	);*/

	//Pre-transposed
	mat4 model(
		yc*zc*sx + xs*ys*zs*sx, xc*zs*sx, -ys*zc*sx + xs*yc*zs*sx, 0,
		xs*ys*zc*sy - yc*zs*sy, xc*zc*sy, xs*yc*zc*sy + ys*zs*sy, 0,
		xc*ys*sz, -xs*sz, xc*yc*sz, 0,
		tx, ty, tz, 1
		);

	//Transpose it for glm :)
	//modelMatrix = transpose(model);
	modelMatrix = model; //<--- pre-transposed for performance

	clean();
}

void MeshDrawer::reset() {
	position = vec3(0.f);
	scale = vec3(1.f);
	rotation = vec3(0.f);

	color = vec4(1.f);
}

void MeshDrawer::checkDirty() {
	if (position != prev_position || scale != prev_scale || rotation != prev_rotation)
		isDirty = true;
}

void MeshDrawer::clean() {
	isDirty = false;
	prev_position = position;
	prev_rotation = rotation;
	prev_scale = scale;
}
//----

void MeshDrawer::updateUniforms() {
	if (autoGenMatrix)
		genModelMatrix();

	//Model
	glUniformMatrix4fv(shader->getUniform("u_model"), 1, false, glm::value_ptr(modelMatrix));

	//Texture
	glUniform1i(shader->getUniform("u_usingTexture"), mesh->texture == nullptr ? 0 : 1);

	//Color
	glUniform4fv(shader->getUniform("u_blendColor"), 1, glm::value_ptr(color));
}

void MeshDrawer::draw() {
	if (mesh == nullptr)
		return;

	Camera::active->updateShader(*shader, "u_camera");

	shader->use();
	updateUniforms();

	mesh->draw();
}