#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"

#include <fstream>
#include <filesystem>
#include <optional>
#include <string_view>
#include <print>

namespace fs = std::filesystem;

Shader::Shader(const std::vector<std::pair<GLenum, fs::path>>& shaders)
{
	if (!Create()) return;

	for (const auto& [type, path] : shaders)
	{
		if (auto result = AttachShader(type, path); result != LoadShaderError::NONE)
		{
			std::println("failed to attach shader {}: {}", path.string(), LoadShaderError_ToString(result));
			glDeleteProgram(m_shaderId);
			m_shaderId = 0;
			return;
		}
	}

	if (!Link())
	{
		glDeleteProgram(m_shaderId);
		m_shaderId = 0;
	}
}
Shader::~Shader()
{
	if (m_shaderId)
	{
		glDeleteProgram(m_shaderId);
		m_shaderId = 0;
	}
}

auto Shader::SetUniform(int i, std::string_view name) const -> void
{
	auto location = GetLocation(name);
	if (location) glUniform1i(*location, i);
}
auto Shader::SetUniform(float f, std::string_view name) const -> void
{
	auto location = GetLocation(name);
	if (location) glUniform1f(*location, f);
}
auto Shader::SetUniform(const glm::vec2& v, std::string_view name) const -> void
{
	auto location = GetLocation(name);
	if (location) glUniform2fv(*location, 1, &v.x);
}
auto Shader::SetUniform(const glm::vec3& v, std::string_view name) const -> void
{
	auto location = GetLocation(name);
	if (location) glUniform3fv(*location, 1, &v.x);
}
auto Shader::SetUniform(const glm::vec4& v, std::string_view name) const -> void
{
	auto location = GetLocation(name);
	if (location) glUniform4fv(*location, 1, &v.x);
}
auto Shader::SetUniform(const glm::mat4& mat, std::string_view name) const -> void
{
	auto location = GetLocation(name);
	if (location) glUniformMatrix4fv(*location, 1, GL_FALSE, &mat[0][0]);
}

auto Shader::Create() -> bool
{
	m_shaderId = glCreateProgram();
	if (!m_shaderId)
	{
		std::println("failed to create shader program");
		return false;
	}
	return true;
}
auto Shader::AttachShader(GLenum type, const fs::path& path) const -> LoadShaderError
{
	GLuint shaderId = glCreateShader(type);
	if (!shaderId) return LoadShaderError::CREATE_PROGRAM;

	auto source = ReadFile(path);
	if (!source) return LoadShaderError::FILE_NOT_FOUND;

	const char* src = source->c_str();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	if (!CheckShader(shaderId))
	{
		glDeleteShader(shaderId);
		return LoadShaderError::COMPILE;
	}

	glAttachShader(m_shaderId, shaderId);
	glDeleteShader(shaderId);

	return LoadShaderError::NONE;
}
auto Shader::AttachShader(GLenum type, std::string_view source) const -> LoadShaderError
{
	GLuint shaderId = glCreateShader(type);
	if (!shaderId) return LoadShaderError::CREATE_PROGRAM;

	const char* src = source.data();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	if (!CheckShader(shaderId))
	{
		glDeleteShader(shaderId);
		return LoadShaderError::COMPILE;
	}

	glAttachShader(m_shaderId, shaderId);
	glDeleteShader(shaderId);

	return LoadShaderError::NONE;
}
auto Shader::CheckShader(GLuint shaderId) const -> bool
{
	GLint success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint logLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetShaderInfoLog(shaderId, logLength, nullptr, log.data());

		std::println("shader compile error: {}", log);
		return false;
	}
	return true;
}
auto Shader::Link() const -> bool
{
	glLinkProgram(m_shaderId);

	GLint infoLogLength = 0, result = 0;
	glGetProgramiv(m_shaderId, GL_LINK_STATUS, &result);
	glGetProgramiv(m_shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!result)
	{
		std::string errorMessage(infoLogLength, '\0');
		glGetProgramInfoLog(m_shaderId, infoLogLength, nullptr, (GLchar*)errorMessage.data());
		std::println("failed to link shader program: {}", errorMessage);
		return false;
	}
	return true;
}

auto Shader::LoadShaderError_ToString(LoadShaderError error) -> std::string_view
{
	switch (error)
	{
		using enum LoadShaderError;

	case CREATE_PROGRAM: return "CREATE_PROGRAM";
	case FILE_NOT_FOUND: return "FILE_NOT_FOUND";
	case COMPILE:        return "COMPILE";
	default: return "UNKNOWN";
	}
}

auto Shader::ReadFile(const fs::path& path) const -> std::optional<std::string>
{
	std::ifstream shaderStream(path);
	if (!shaderStream) return std::nullopt;

	std::stringstream buffer;
	buffer << shaderStream.rdbuf();
	auto shaderSource = buffer.str();

	shaderStream.close();
	return shaderSource;
}
auto Shader::GetLocation(std::string_view name) const -> std::optional<int>
{
	int location = glGetUniformLocation(m_shaderId, name.data());
	return location >= 0
		? std::optional<int>{ location }
	: std::nullopt;
}