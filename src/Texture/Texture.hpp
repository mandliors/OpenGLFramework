#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class Texture
{
public:
	Texture(const fs::path &path, bool transparent = false, GLint sampling = GL_LINEAR);
	Texture(uint32_t width, uint32_t height, const std::vector<glm::vec3> &image, GLint sampling = GL_LINEAR);
	Texture(uint32_t width, uint32_t height, const std::vector<glm::vec4> &image, GLint sampling = GL_LINEAR);
	Texture(uint32_t width, uint32_t height, const glm::vec3 &color)
		: Texture(width, height, std::vector<glm::vec3>(width * height, color))
	{
	}
	~Texture()
	{
		if (m_textureId > 0)
			glDeleteTextures(1, &m_textureId);
	}

	auto Bind(int textureUnit) -> void;

private:
	unsigned m_textureId = 0;
};