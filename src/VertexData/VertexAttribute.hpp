#pragma once

#include "glad/glad.h"

struct VertexAttribute
{
	GLuint location;
	GLint size;
	GLenum type;
	GLboolean normalized;
	size_t offset;
};