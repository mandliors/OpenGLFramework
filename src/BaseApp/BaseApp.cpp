#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "BaseApp.hpp"

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

	return true;
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

	glfwDestroyWindow(m_window);
	glfwTerminate();
}