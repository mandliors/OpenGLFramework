#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <numbers>

class Camera
{
public:
	Camera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up,
		float fov, float aspect, float near, float far)
		: m_eye(pos), m_dir(glm::normalize(lookAt - pos)), m_up(up),
		m_fov(fov* std::numbers::pi_v<float> / 180), m_aspect(aspect), m_near(near), m_far(far) {
	}

	Camera(const glm::vec3& pos, const glm::vec3& lookAt, float fov, float aspect)
		: Camera(pos, lookAt, glm::vec3{ 0.0f, 1.0f, 0.0f }, fov, aspect, 0.1f, 100.0f) {
	}

	auto GetPosition() const -> glm::vec3 { return m_eye; }

	auto Resize(float aspect) -> void { m_aspect = aspect; }
	auto Move(const glm::vec3& delta) -> void { m_eye += delta; }

	auto GetView() const -> glm::mat4 { return glm::lookAt(m_eye, m_eye + m_dir, m_up); }
	auto GetProjection() const -> glm::mat4 { return glm::perspective(m_fov, m_aspect, m_near, m_far); }

private:
	glm::vec3 m_eye;
	glm::vec3 m_dir;
	glm::vec3 m_up;

	float m_fov;
	float m_aspect;
	float m_near, m_far;
};