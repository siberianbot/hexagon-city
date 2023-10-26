#ifndef HEXAGON_CITY_RENDERING_GRID_DRAWABLE_PROVIDER_HPP
#define HEXAGON_CITY_RENDERING_GRID_DRAWABLE_PROVIDER_HPP

#include <cstdint>
#include <memory>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridCellComponent.hpp"

using namespace Penrose;

class GridDrawableProvider : public Resource<GridDrawableProvider, ResourceGroup::Rendering>,
                             public DrawableProvider {
public:
    explicit GridDrawableProvider(ResourceSet *resources);
    ~GridDrawableProvider() override = default;

    [[nodiscard]] std::vector<Drawable> getDrawablesFor(const Entity &entity) override;

private:
    ResourceProxy<EntityManager> _entityManager;

    [[nodiscard]] std::optional<Drawable> createDrawable(Entity entity,
                                                         const std::shared_ptr<GridBuildingComponent> &gridBuilding);
    [[nodiscard]] std::optional<Drawable> createDrawable(Entity entity,
                                                         const std::shared_ptr<GridCellComponent> &gridCell);

    [[nodiscard]] static glm::mat4 getModel(std::int32_t row, std::int32_t column, float width, float height);
};

#endif // HEXAGON_CITY_RENDERING_GRID_DRAWABLE_PROVIDER_HPP
