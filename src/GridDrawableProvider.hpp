#ifndef HEXAGON_CITY_GRID_DRAWABLE_PROVIDER_HPP
#define HEXAGON_CITY_GRID_DRAWABLE_PROVIDER_HPP

#include <cstdint>
#include <memory>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/GridBuildingComponent.hpp"
#include "src/GridCellComponent.hpp"

using namespace Penrose;

class GridDrawableProvider : public Resource<GridDrawableProvider>,
                             public DrawableProvider {
public:
    explicit GridDrawableProvider(ResourceSet *resources);
    ~GridDrawableProvider() override = default;

    [[nodiscard]] std::vector<Drawable> getDrawablesFor(const Entity &entity) override;

private:
    ResourceProxy<ECSManager> _ecsManager;

    [[nodiscard]] std::optional<Drawable> createDrawable(Entity entity,
                                                         const std::shared_ptr<GridBuildingComponent> &gridBuilding);
    [[nodiscard]] std::optional<Drawable> createDrawable(Entity entity,
                                                         const std::shared_ptr<GridCellComponent> &gridCell);

    [[nodiscard]] static glm::mat4 getModel(std::int32_t row, std::int32_t column, float width, float height);
};

#endif // HEXAGON_CITY_GRID_DRAWABLE_PROVIDER_HPP
