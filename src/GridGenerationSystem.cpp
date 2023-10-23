#include "GridGenerationSystem.hpp"

#include "src/GridCellComponent.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/RayCollisionVolumeComponent.hpp"

constexpr static const std::int32_t GRID_RADIUS = 16;
constexpr static const CubeCoord GRID_CENTER = {0, 0, 0};

GridGenerationSystem::GridGenerationSystem(ResourceSet *resources)
        : _ecsManager(resources->get<ECSManager>()),
          _randomGenerator(resources->get<RandomGenerator>()),
          _sceneManager(resources->get<SceneManager>()) {
    //
}

void GridGenerationSystem::init() {
    auto root = this->_sceneManager->getOrAddRoot("City");

    auto pushCell = [&](const CubeCoord &cube) {
        auto type = this->_randomGenerator->generateInt(0, 3);
        auto entity = this->_ecsManager->createEntity();

        auto position = this->_ecsManager->addComponent<GridPositionComponent>(entity);
        position->fromCubeCoordinates(cube);

        auto cell = this->_ecsManager->addComponent<GridCellComponent>(entity);
        cell->type() = static_cast<GridCellType>(type);
        cell->building() = std::nullopt;

        this->_ecsManager->addComponent<RayCollisionVolumeComponent>(entity);
        this->_sceneManager->insertEntityNode(root, entity);
    };

    pushCell(GRID_CENTER);

    for (std::int32_t r = 1; r <= GRID_RADIUS; r++) {
        CubeCoord hex = cubeAdd(GRID_CENTER, cubeScale(CUBE_DIRECTIONS.at(4), r));

        for (std::int32_t i = 0; i < 6; i++) {
            for (std::int32_t j = 0; j < r; j++) {
                pushCell(hex);

                hex = cubeNeighbor(hex, i);
            }
        }
    }
}
