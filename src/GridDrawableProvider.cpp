#include "GridDrawableProvider.hpp"

#include <cmath>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/GridConstants.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/HoveredComponent.hpp"
#include "src/SelectedComponent.hpp"

GridDrawableProvider::GridDrawableProvider(ResourceSet *resources)
        : _ecsManager(resources->get<ECSManager>()) {
    //
}

std::vector<Drawable> GridDrawableProvider::getDrawablesFor(const Entity &entity) {

    auto maybeCell = this->_ecsManager->tryGetComponent<GridCellComponent>(entity);

    if (!maybeCell.has_value()) {
        return {};
    }

    auto maybeCellDrawable = this->createDrawable(entity, *maybeCell);

    if (!maybeCellDrawable.has_value()) {
        return {};
    }

    if ((*maybeCell)->building().has_value()) {

        auto building = *(*maybeCell)->building();
        auto maybeBuildingDrawable = this->createDrawable(building,
                                                          this->_ecsManager->getComponent<GridBuildingComponent>(
                                                                  building));

        if (maybeBuildingDrawable.has_value()) {
            return {
                    *maybeCellDrawable,
                    *maybeBuildingDrawable
            };
        }
    }

    return {
            *maybeCellDrawable
    };
}

std::optional<Drawable> GridDrawableProvider::createDrawable(Entity entity,
                                                             const std::shared_ptr<GridBuildingComponent> &gridBuilding) {

    auto maybePosition = this->_ecsManager->tryGetComponent<GridPositionComponent>(entity);

    if (!maybePosition.has_value()) {
        return std::nullopt;
    }

    auto drawable = Drawable{
            .entity = entity,
            .meshAsset = "models/building.asset",
            .albedoTextureAsset = "textures/building.asset",
            .model = GridDrawableProvider::getModel((*maybePosition)->row(),
                                                    (*maybePosition)->column(),
                                                    0.75, gridBuilding->level() * GRID_BUILDING_PER_LEVEL_HEIGHT),
            .modelRot = glm::mat4(1),
            .color = GRID_BUILDING_COLORS[static_cast<std::uint32_t>(gridBuilding->type())]
    };

    if (this->_ecsManager->hasComponent<HoveredComponent>(entity)) {
        drawable.color = GRID_HOVERED_COLOR;
    }

    if (this->_ecsManager->hasComponent<SelectedComponent>(entity)) {
        drawable.color = GRID_SELECTED_COLOR;
    }

    return drawable;
}

std::optional<Drawable> GridDrawableProvider::createDrawable(Entity entity,
                                                             const std::shared_ptr<GridCellComponent> &gridCell) {

    auto maybePosition = this->_ecsManager->tryGetComponent<GridPositionComponent>(entity);

    if (!maybePosition.has_value()) {
        return std::nullopt;
    }

    auto drawable = Drawable{
            .entity = entity,
            .meshAsset = "models/building.asset",
            .albedoTextureAsset = "textures/building.asset",
            .model = GridDrawableProvider::getModel((*maybePosition)->row(),
                                                    (*maybePosition)->column(),
                                                    1, GRID_CELL_HEIGHT),
            .modelRot = glm::mat4(1),
            .color = GRID_CELL_COLORS[static_cast<std::uint32_t>(gridCell->type())]
    };

    if (this->_ecsManager->hasComponent<HoveredComponent>(entity)) {
        drawable.color = GRID_HOVERED_COLOR;
    }

    if (this->_ecsManager->hasComponent<SelectedComponent>(entity)) {
        drawable.color = GRID_SELECTED_COLOR;
    }

    return drawable;
}

glm::mat4 GridDrawableProvider::getModel(std::int32_t row, std::int32_t column, float width, float height) {
    auto pos = GRID_CELL_DIAMETER * glm::vec3(sqrt(3) * column + sqrt(3) / 2 * row,
                                              0,
                                              3. / 2 * row);
    auto scale = glm::vec3(width, height, width);

    return glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), scale);
}
