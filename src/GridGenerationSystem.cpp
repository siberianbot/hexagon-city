#include "GridGenerationSystem.hpp"

#include <random>

#include "src/GridCellComponent.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/RayCollisionVolumeComponent.hpp"

constexpr static const std::int32_t GRID_RADIUS = 16;
constexpr static const CubeCoord GRID_CENTER = {0, 0, 0};

GridGenerationSystem::GridGenerationSystem(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()),
          _sceneManager(resources->getLazy<SceneManager>()) {
    //
}

void GridGenerationSystem::init() {
    auto root = this->_sceneManager->getOrAddRoot("City");

    auto randomDevice = std::random_device();
    auto randomEngine = std::default_random_engine(randomDevice());
    auto distribution = std::uniform_int_distribution(0, 3);

    auto pushCell = [&](const CubeCoord &cube) {
        auto type = distribution(randomEngine);
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
