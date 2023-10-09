#include "GridDrawableProvider.hpp"

#include <cmath>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/GridBuildingComponent.hpp"
#include "src/GridCellComponent.hpp"
#include "src/GridConstants.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/HoveredComponent.hpp"
#include "src/SelectedComponent.hpp"

GridDrawableProvider::GridDrawableProvider(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()) {
    //
}

std::vector<Drawable> GridDrawableProvider::getDrawablesFor(const Entity &entity) {

    auto maybeGridPosition = this->_ecsManager->tryGetComponent<GridPositionComponent>(entity);

    if (!maybeGridPosition.has_value()) {
        return {};
    }

    auto drawable = Drawable{
            .entity = entity,
            .meshAsset = "models/building.asset",
            .albedoTextureAsset = "textures/building.asset",
            .model = glm::mat4(1),
            .modelRot = glm::mat4(1),
            .color = glm::vec3(1)
    };

    auto maybeBuilding = this->_ecsManager->tryGetComponent<GridBuildingComponent>(entity);
    auto maybeCell = this->_ecsManager->tryGetComponent<GridCellComponent>(entity);

    if (maybeBuilding.has_value()) {
        drawable.model = GridDrawableProvider::getModel((*maybeGridPosition)->row(),
                                                        (*maybeGridPosition)->column(),
                                                        (*maybeBuilding)->level() * GRID_BUILDING_PER_LEVEL_HEIGHT);

        drawable.color = GRID_BUILDING_COLOR;
    }

    if (maybeCell.has_value()) {
        drawable.model = GridDrawableProvider::getModel((*maybeGridPosition)->row(),
                                                        (*maybeGridPosition)->column(),
                                                        GRID_CELL_HEIGHT);

        drawable.color = GRID_CELL_COLORS[static_cast<std::uint32_t>((*maybeCell)->type())];
    }

    if (this->_ecsManager->hasComponent<HoveredComponent>(entity)) {
        drawable.color = GRID_HOVERED_COLOR;
    }

    if (this->_ecsManager->hasComponent<SelectedComponent>(entity)) {
        drawable.color = GRID_SELECTED_COLOR;
    }

    return {drawable};
}

glm::mat4 GridDrawableProvider::getModel(std::int32_t row, std::int32_t column, float height) {
    auto pos = GRID_CELL_DIAMETER * glm::vec3(sqrt(3) * column + sqrt(3) / 2 * row,
                                              0,
                                              3. / 2 * row);
    auto scale = glm::vec3(1, height, 1);

    return glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), scale);
}
