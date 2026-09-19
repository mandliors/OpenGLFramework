#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "BaseApp/BaseApp.hpp"
#include "Camera/Camera.hpp"
#include "MeshRenderer/Shaders/FaceShader.hpp"
#include "MeshRenderer/Shaders/EdgeShader.hpp"
#include "MeshRenderer/Shaders/PointShader.hpp"
#include "MeshRenderer/MeshRenderer.hpp"
#include "Framebuffer/Framebuffer.hpp"
#include "Renderables/Renderable3D.hpp"

#include <vector>
#include <memory>
#include <string>

typedef OpenMesh::PolyMesh_ArrayKernelT<> PolyMesh;

class App : public BaseApp
{
public:
	App(uint32_t width, uint32_t height, std::string_view title);

	auto OnInit() -> void override;
	auto OnUpdate() -> void override;
	auto OnRender() -> void override;
	auto OnTimeElapsed(double dt) -> void override;

private:
	auto OnImGuiRender() -> void override;

	auto LoadModel(std::string_view path) -> std::unique_ptr<PolyMesh>;

	auto ScreenToNDC(int x, int y) const -> glm::vec2;

private:
	std::unique_ptr<FaceShader> m_faceShader;
	std::unique_ptr<EdgeShader> m_edgeShader;
	std::unique_ptr<PointShader> m_pointShader;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Framebuffer> m_framebuffer;

	Material m_material;
	std::vector<Light> m_lights;
	RenderState m_renderState;

	std::unique_ptr<PolyMesh> m_topologyMesh;
	std::unique_ptr<MeshRenderer> m_renderMesh;
	float m_rotation = 0.0f;
	glm::quat m_quat = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};

	bool m_showDebug = true;
};