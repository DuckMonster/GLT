#include "Stencil.hpp"

using namespace glt;

void Stencil::start() {
	glEnable(GL_STENCIL_TEST);
}

void Stencil::end() {
	glDisable(GL_STENCIL_TEST);
	//setColorDraw(true);
}

void Stencil::func(GLenum func, GLuint value) {
	glStencilFunc(func, value, 0xFF);
}

void Stencil::op(GLenum sFail, GLenum dFail, GLenum pass) {
	glStencilOp(sFail, dFail, pass);
}

void Stencil::clear() {
	glClear(GL_STENCIL_BUFFER_BIT);
}

void Stencil::setColorDraw(bool enabled) {
	GLboolean mask = (enabled ? GL_TRUE : GL_FALSE);
	glColorMask(mask, mask, mask, mask);
}