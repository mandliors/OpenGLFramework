#pragma once

#include "Rendering/RenderState.hpp"
#include "MeshRenderer/SpecialVertexDatas.hpp"

class Renderable
{
public:
    Renderable(Shader &shader, GLenum type, const glm::vec4 &color)
        : m_shader(shader), m_type(type), m_color(color)
    {
    }
    virtual ~Renderable() = default;

    virtual auto Draw(const RenderState &renderState) const -> void = 0;

    auto SetType(GLenum type) -> void { m_type = type; }
    auto GetType() const -> GLenum { return m_type; }

    auto SetColor(const glm::vec4 &color) -> void { m_color = color; }
    auto GetColor() const & -> const glm::vec4 & { return m_color; }

    auto SetShader(Shader &shader) -> void { m_shader = shader; }
    auto GetShader() const & -> Shader & { return m_shader; }

protected:
    Shader &m_shader;
    GLenum m_type;
    glm::vec4 m_color;
};