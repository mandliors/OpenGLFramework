#include "DebugRenderer.hpp"

std::unordered_map<std::string, std::unique_ptr<Renderable>> DebugRenderer::s_renderables;

auto DebugRenderer::Add(std::string_view name, std::unique_ptr<Renderable> renderable) -> void
{
    s_renderables.emplace(name, std::move(renderable));
}
auto DebugRenderer::Get(std::string_view name) -> Renderable *
{
    auto it = s_renderables.find(std::string(name));
    if (it != s_renderables.end())
        return it->second.get();

    return nullptr;
}
auto DebugRenderer::Remove(std::string_view name) -> void
{
    s_renderables.erase(std::string(name));
}
auto DebugRenderer::Clear() -> void
{
    s_renderables.clear();
}
auto DebugRenderer::DrawAll(const RenderState &renderState) -> void
{
    glDisable(GL_DEPTH_TEST);
    for (const auto &[name, renderable] : s_renderables)
        renderable->Draw(renderState);
    glEnable(GL_DEPTH_TEST);
}