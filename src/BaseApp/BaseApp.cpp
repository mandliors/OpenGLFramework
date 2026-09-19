#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "BaseApp.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include <chrono>
#include <thread>
#include <print>

auto BaseApp::Init() -> bool
{
	glfwSetErrorCallback([](int error, const char *description)
						 { std::println("error: {}", description); });
	if (!glfwInit())
	{
		std::println("failed to initialize GLFW");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, s_majorNumber);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, s_minorNumber);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		std::println("failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwSetWindowUserPointer(m_window, this);

	glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
					   {
			auto self = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
			if (!self) [[unlikely]] return;

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				return;
			}
			if (action == GLFW_PRESS) self->OnKeyPressed(key, mods);
			if (action == GLFW_RELEASE) self->OnKeyReleased(key, mods); });
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods)
							   {
			auto self = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
			if (!self) [[unlikely]] return;

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			if (action == GLFW_PRESS) self->OnMousePressed(button, static_cast<uint32_t>(x), static_cast<uint32_t>(y));
			else self->OnMouseReleased(button, static_cast<uint32_t>(x), static_cast<uint32_t>(y)); });
	glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double x, double y)
							 {
			auto self = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
			if (self) [[likely]] self->OnMouseMotion(static_cast<uint32_t>(x), static_cast<uint32_t>(y)); });
	glfwSetWindowRefreshCallback(m_window, [](GLFWwindow *window)
								 {
			auto self = static_cast<BaseApp*>(glfwGetWindowUserPointer(window));
			if (self) [[likely]] self->m_screenRefresh = true; });
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
								   { glViewport(0, 0, width, height); });

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::print("failed to initialize GLAD");
		return false;
	}
	glfwSwapInterval(1);

	OnInit();
	OnImGuiInit();
	SetUiScale(1.0f);

	return true;
}
auto BaseApp::SetUiScale(float scale) -> void
{
	ImGuiStyle &style = ImGui::GetStyle();
	style.FontScaleDpi = scale;

	static ImGuiStyle baseStyle = style;
	ImGuiStyle scaledStyle = baseStyle;
	scaledStyle.FontScaleDpi = scale;
	scaledStyle.ScaleAllSizes(scale);
	style = scaledStyle;
}
auto BaseApp::Run() -> void
{
	using clock = std::chrono::steady_clock;
	using seconds = std::chrono::duration<double>;

	auto prevTime = clock::now();

	while (!glfwWindowShouldClose(m_window))
	{
		auto frameStart = clock::now();

		OnUpdate();
		if (m_screenRefresh)
		{
			OnRender();
			OnImGuiRender();
			glfwSwapBuffers(m_window);
			m_screenRefresh = false;
		}

		glfwPollEvents();

		if (m_targetFrameTime > 0.0)
		{
			auto frameTime = clock::now() - frameStart;
			auto targetFrameTime = seconds(m_targetFrameTime);

			if (frameTime < targetFrameTime)
			{
				std::this_thread::sleep_for(targetFrameTime - frameTime);
				m_screenRefresh = true;
			}
		}

		auto now = clock::now();
		auto dt = seconds(now - prevTime).count();
		prevTime = now;

		OnTimeElapsed(dt);
	}
}
auto BaseApp::Destroy() -> void
{
	OnDestroy();
	OnImGuiDestroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

auto BaseApp::OnImGuiInit() -> void
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDpiScaleFonts = false;
	io.ConfigDpiScaleViewports = false;

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
}
auto BaseApp::OnImGuiRender() -> void
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

	// GUI rendering goes here

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
auto BaseApp::OnImGuiDestroy() -> void
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}