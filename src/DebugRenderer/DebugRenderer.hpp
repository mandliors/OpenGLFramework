#include "Renderables/Renderable.hpp"

#include <string_view>
#include <unordered_map>

class DebugRenderer
{
public:
    static auto Add(std::string_view name, std::unique_ptr<Renderable> renderable) -> void;
    static auto Get(std::string_view name) -> Renderable *;
    static auto Remove(std::string_view name) -> void;
    static auto Clear() -> void;

    static auto DrawAll(const RenderState &renderState) -> void;

private:
    static std::unordered_map<std::string, std::unique_ptr<Renderable>> s_renderables;
};