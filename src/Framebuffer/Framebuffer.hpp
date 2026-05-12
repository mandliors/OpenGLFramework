#pragma once

#include <memory>

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height, uint32_t samples = 1, bool swapChainTarget = false);
	~Framebuffer();

	auto Invalidate() -> void;
	auto Resize(uint32_t width, uint32_t height) -> void;
	auto Bind() -> void;
	auto Unbind() -> void;

	auto GetColorAttachmentRendererID() const -> uint32_t;

private:
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_Samples;
	bool m_SwapChainTarget;

	uint32_t m_RendererID;
	uint32_t m_ColorAttachment;
	uint32_t m_DepthAttachment;
};