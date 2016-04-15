#include "TextMesh.hpp"

#include <fstream>
#include <sstream>

using namespace glt;
using namespace std;

TextMesh::TextMesh() {
	mesh.drawMode = GL_QUADS;
}
TextMesh::TextMesh(Font* font) {
	mesh.drawMode = GL_QUADS;
	setFont(font);
}

void TextMesh::setText(std::string text) {
	if (this->text == text)
		return;

	this->text = text;
	updateMesh();
}

void TextMesh::setFont(Font* font) {
	if (this->font == font)
		return;

	this->font = font;
	updateMesh();
}

void TextMesh::setHAlign(int hAlign) {
	this->hAlign = glm::clamp(hAlign, -1, 1);
	updateMesh();
}
void TextMesh::setVAlign(int vAlign) {
	this->vAlign = glm::clamp(vAlign, -1, 1);
	updateMesh();
}

int TextMesh::getHAlign() {
	return hAlign;
}
int TextMesh::getVAlign() {
	return vAlign;
}

void TextMesh::setSpacing(float spacing) {
	if (this->spacing == spacing)
		return;

	this->spacing = spacing;
	updateMesh();
}

Mesh* TextMesh::getMesh() {
	return &mesh;
}

void TextMesh::updateMesh() {
	if (font == nullptr) {
		mesh.setVertices(nullptr, 0);
		return;
	}

	//Load chars
	vector<Font::CharInfo> chars = font->getString(text);

	//Verts and UV arrays
	float* vertices = new float[chars.size() * 4 * 3];
	float* uvs = new float[chars.size() * 4 * 2];

	//Get the totalwidth, by just adding together all the aspect values
	float totalWidth = 0.f;
	for (int i = 0; i < chars.size(); i++)
		totalWidth += chars[i].aspect + spacing;

	//Set offsets
	float xOffset = (0.5f + 0.5f * hAlign) * -totalWidth;
	float yOffset = 0.5f * vAlign;

	for (int i = 0; i < chars.size(); i++) {
		//Get char
		Font::CharInfo info = chars[i];
		float w = info.aspect;
		float h = 0.5f;

		//Get indices in array
		int vIndex = i * 12;
		int tIndex = i * 8;

		//Bottom left
		vertices[vIndex] = 0.f + xOffset;
		vertices[vIndex + 1] = -h + yOffset;
		vertices[vIndex + 2] = 0.f;

		//Bottom right
		vertices[vIndex + 3] = w + xOffset;
		vertices[vIndex + 4] = -h + yOffset;
		vertices[vIndex + 5] = 0.f;

		//Top right
		vertices[vIndex + 6] = w + xOffset;
		vertices[vIndex + 7] = h + yOffset;
		vertices[vIndex + 8] = 0.f;

		//Top left
		vertices[vIndex + 9] = 0.f + xOffset;
		vertices[vIndex + 10] = h + yOffset;
		vertices[vIndex + 11] = 0.f;

		//Increase the xOffset for the next character
		xOffset += chars[i].aspect + spacing;

		//UV values
		float uvx = info.x;
		float uvy = info.y;
		float uvw = info.width;
		float uvh = info.height;

		//Bottom left
		uvs[tIndex + 0] = uvx;
		uvs[tIndex + 1] = uvy + uvh;

		//Bottom right
		uvs[tIndex + 2] = uvx + uvw;
		uvs[tIndex + 3] = uvy + uvh;

		//Top right
		uvs[tIndex + 4] = uvx + uvw;
		uvs[tIndex + 5] = uvy;

		//Top left
		uvs[tIndex + 6] = uvx;
		uvs[tIndex + 7] = uvy;
	}

	mesh.setVertices(vertices, chars.size() * 4 * 3 * sizeof(float));
	mesh.setUVS(uvs, chars.size() * 4 * 2 * sizeof(float));

	delete[] vertices;
	delete[] uvs;	
}

//------------