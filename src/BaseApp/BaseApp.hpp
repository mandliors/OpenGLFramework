#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <print>
#include <chrono>

class BaseApp
{
public:
	BaseApp(uint32_t width, uint32_t height, std::string_view title)
		: m_width(width), m_height(height), m_title(title)
	{
	}
	virtual ~BaseApp() = default;

	auto Init() -> bool;
	auto SetTargetFPS(uint32_t fps) -> void { m_targetFrameTime = fps > 0 ? 1.0 / fps : -1.0; }
	auto Run() -> void;
	auto Destroy() -> void;

protected:
	virtual auto OnInit() -> void {}
	virtual auto OnUpdate() -> void {}
	virtual auto OnRender() -> void {}
	virtual auto OnDestroy() -> void {}

	virtual auto OnKeyPressed(uint32_t key, uint32_t mods) -> void {}
	virtual auto OnKeyReleased(uint32_t key, uint32_t mods) -> void {}
	virtual auto OnMousePressed(uint32_t button, uint32_t x, uint32_t y) -> void {}
	virtual auto OnMouseReleased(uint32_t button, uint32_t x, uint32_t y) -> void {}
	virtual auto OnMouseMotion(int x, int y) -> void {}

	virtual auto OnTimeElapsed([[maybe_unused]] double dt) -> void {}

	auto Invalidate() -> void { m_screenRefresh = true; }

private:
	static constexpr uint32_t s_majorNumber = 4;
	static constexpr uint32_t s_minorNumber = 6;

protected:
	uint32_t m_width;
	uint32_t m_height;

	std::string m_title;
	GLFWwindow *m_window = nullptr;

private:
	bool m_screenRefresh = true;

	double m_targetFrameTime = -1.0;
};