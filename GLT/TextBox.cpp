#include "TextBox.hpp"
using namespace glt;
using namespace std;

TextBox::TextBox() {
	setMesh(textMesh.getMesh());
}

TextBox::TextBox(std::string text) {
	this->text = text;
}
TextBox::TextBox(Font* font) {
	this->font = font;
}
TextBox::TextBox(std::string text, Font* font) {
	this->text = text;
	this->font = font;
}

void TextBox::draw() {
	if (font == nullptr)
		return;

	texture = font->getTexture();
	textMesh.setFont(font);
	textMesh.setText(text);

	MeshDrawer::draw();
}