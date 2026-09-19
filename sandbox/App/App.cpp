#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "App.hpp"
#include "Renderables/Renderable3D.hpp"
#include "DebugRenderer/DebugRenderer.hpp"

#include <array>
#include <ranges>
#include <iostream>

namespace fs = std::filesystem;

App::App(uint32_t width, uint32_t height, std::string_view title)
	: BaseApp(width, height, title)
{
}

auto App::OnInit() -> void
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	static const float pointSize = 10.0f;
	static const float lineWidth = 5.0f;

	glPointSize(pointSize);
	glLineWidth(lineWidth);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_faceShader = std::make_unique<FaceShader>();
	m_edgeShader = std::make_unique<EdgeShader>();
	m_pointShader = std::make_unique<PointShader>();

	m_camera = std::make_unique<Camera>(
		glm::vec3{0.0f, 1.5f, 4.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 1.0f, 0.0f},
		45.0f,
		static_cast<float>(m_width) / static_cast<float>(m_height),
		1.0f,
		20.0f);

	m_material = Material{
		glm::vec3{0.4f},
		glm::vec3{0.8f},
		glm::vec3{0.2f},
		2.0f,
		0.0f};
	m_lights = std::vector<Light>{
		Light{
			glm::vec4{2.0f, 1.0f, 2.0f, 0.0f},
			glm::vec3{2.0f},
			glm::vec3{1.0f},
			1.0, 0.14f, 0.07f}};

	m_framebuffer = std::make_unique<Framebuffer>(m_width, m_height, 1, false);

	static const auto purple = glm::vec4{0.55f, 0.3f, 1.0f, 1.0f};
	static const auto black = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};

	m_topologyMesh = LoadModel("assets/models/bunny.obj");
	m_renderMesh = std::make_unique<MeshRenderer>(*m_topologyMesh, *m_faceShader, *m_edgeShader, *m_pointShader, purple, black, purple);
	m_renderMesh->SetDrawMode(true, true, false);

	auto lines = std::make_unique<Renderable3D>(*m_edgeShader, GL_LINES, glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
	size_t count = 0;
	for (auto fh : m_topologyMesh->faces())
	{
		if (count++ >= 10)
			break;

		std::vector<glm::vec3> facePoints;
		for (auto fvh : m_topologyMesh->fv_range(fh))
		{
			const auto &p = m_topologyMesh->point(fvh);
			facePoints.emplace_back(p[0], p[1], p[2]);
		}

		const std::size_t numVerts = facePoints.size();
		for (std::size_t i = 0; i < numVerts; ++i)
		{
			lines->Vtx().push_back(facePoints[i]);
			lines->Vtx().push_back(facePoints[(i + 1) % numVerts]);
		}
	}

	lines->UpdateGPU();
	DebugRenderer::Add("debug", std::move(lines));
}
auto App::OnUpdate() -> void
{
	static const float modelScale = 12.0f;
	static const glm::vec3 modelPosition = glm::vec3{0.4f, -0.9f, 0.0f};

	auto M = glm::translate(glm::mat4{1.0f}, modelPosition) * glm::mat4_cast(m_quat) * glm::scale(glm::mat4{1.0f}, glm::vec3{modelScale});
	auto Minv = glm::inverse(M);
	auto V = m_camera->GetView();
	auto P = m_camera->GetProjection();
	auto MVP = P * V * M;

	m_renderState =
		{
			.MVP = MVP,
			.Model = M,
			.ModelInverse = Minv,
			.View = V,
			.Projection = P,
			.Material = &m_material,
			.Lights = &m_lights,
			.Texture = nullptr,
			.CameraPosition = m_camera->GetPosition()};
}
auto App::OnRender() -> void
{
	m_framebuffer->Bind();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_renderMesh)
		m_renderMesh->Draw(m_renderState);

	DebugRenderer::DrawAll(m_renderState);

	m_framebuffer->Unbind();
}

auto App::OnTimeElapsed(double dt) -> void
{
	m_rotation += static_cast<float>(dt) * 0.5f;
	m_quat = glm::angleAxis(m_rotation, glm::vec3{0.0f, 1.0f, 0.0f});
}

auto App::OnImGuiRender() -> void
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (const auto &io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Viewport");
	{
		bool windowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

		static auto s_viewportSize = glm::vec2{0.0f, 0.0f};
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if ((s_viewportSize != *((glm::vec2 *)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0))
		{
			s_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
			m_framebuffer->Resize((uint32_t)s_viewportSize.x, (uint32_t)s_viewportSize.y);
		}
		m_camera->Resize(viewportPanelSize.x / viewportPanelSize.y);
		ImGui::Image(reinterpret_cast<void *>(m_framebuffer->GetColorAttachmentRendererID()), ImVec2{s_viewportSize.x, s_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

		bool imageHovered = ImGui::IsItemHovered();
	}
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto App::LoadModel(std::string_view path) -> std::unique_ptr<PolyMesh>
{
	auto mesh = std::make_unique<PolyMesh>();

	OpenMesh::IO::Options opt;
	opt += OpenMesh::IO::Options::VertexNormal;
	opt += OpenMesh::IO::Options::FaceNormal;

	if (!OpenMesh::IO::read_mesh(*mesh, std::string{path}, opt))
		std::cerr << "failed to load model\n";
	else // if the file didn't contain normals (we need per-vertex normals)
	{
		if (!mesh->has_vertex_normals())
		{
			mesh->request_vertex_normals();
			mesh->update_normals();
		}
		if (!mesh->has_face_normals())
		{
			mesh->request_face_normals();
			mesh->update_face_normals();
		}
		mesh->request_face_status();
		mesh->request_edge_status();
		mesh->request_vertex_status();
	}

	return mesh;
}

auto App::ScreenToNDC(int x, int y) const -> glm::vec2
{
	auto win = ImGui::FindWindowByName("Viewport");
	if (!win)
		return glm::vec2{0.0f};

	auto contentMin = ImVec2{
		win->Pos.x + win->WindowPadding.x,
		win->Pos.y + win->TitleBarHeight + win->WindowPadding.y};

	ImVec2 contentSize = win->ContentRegionRect.GetSize();

	auto localX = static_cast<float>(x) - contentMin.x;
	auto localY = static_cast<float>(y) - contentMin.y;

	auto w = contentSize.x;
	auto h = contentSize.y;

	auto ndcX = 2.0f * (localX / w) - 1.0f;
	auto ndcY = -2.0f * (localY / h) + 1.0f;

	return glm::vec2{ndcX, ndcY};
}