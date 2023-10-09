#ifndef HEXAGON_CITY_GRID_DRAWABLE_PROVIDER_HPP
#define HEXAGON_CITY_GRID_DRAWABLE_PROVIDER_HPP

#include <cstdint>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

using namespace Penrose;

class GridDrawableProvider : public Resource, public DrawableProvider {
public:
    explicit GridDrawableProvider(ResourceSet *resources);
    ~GridDrawableProvider() override = default;

    [[nodiscard]] std::vector<Drawable> getDrawablesFor(const Entity &entity) override;

private:
    Lazy<ECSManager> _ecsManager;

    [[nodiscard]] static glm::mat4 getModel(std::int32_t row, std::int32_t column, float height) ;
};

#endif // HEXAGON_CITY_GRID_DRAWABLE_PROVIDER_HPP
