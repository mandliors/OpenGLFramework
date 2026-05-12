#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Rendering/RenderState.hpp"

#include <filesystem>
#include <fstream>
#include <optional>
#include <string_view>

namespace fs = std::filesystem;

class Shader
{
public:
	Shader() = default;
	explicit Shader(const std::vector<std::pair<GLenum, fs::path>>& shaders);
	explicit Shader(const std::vector<std::pair<GLenum, std::string_view>>& shaders);
	~Shader();

	auto Use() const -> void { if (m_shaderId) glUseProgram(m_shaderId); }

	auto SetUniform(int i, std::string_view name) const -> void;
	auto SetUniform(float f, std::string_view name) const -> void;
	auto SetUniform(const glm::vec2& v, std::string_view name) const -> void;
	auto SetUniform(const glm::vec3& v, std::string_view name) const -> void;
	auto SetUniform(const glm::vec4& v, std::string_view name) const -> void;
	auto SetUniform(const glm::mat4& mat, std::string_view name) const -> void;

	virtual auto Bind(const RenderState& state, bool bindLights = true) const -> void {}

	operator GLuint() const { return m_shaderId; }

protected:
	enum class LoadShaderError { NONE = 0, CREATE_PROGRAM, FILE_NOT_FOUND, COMPILE };

protected:
	auto Create() -> bool;
	auto AttachShader(GLenum type, const fs::path& path) const -> LoadShaderError;
	auto AttachShader(GLenum type, std::string_view source) const -> LoadShaderError;
	auto CheckShader(GLuint shaderId) const -> bool;
	auto Link() const -> bool;

protected:
	static auto LoadShaderError_ToString(LoadShaderError error) -> std::string_view;

private:
	auto ReadFile(const fs::path& path) const -> std::optional<std::string>;
	auto GetLocation(std::string_view name) const -> std::optional<int>;

protected:
	GLuint m_shaderId;
};