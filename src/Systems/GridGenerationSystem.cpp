#include "GridGenerationSystem.hpp"

#include "src/Components/GridCellComponent.hpp"
#include "src/Components/GridPositionComponent.hpp"
#include "src/Components/RayCollisionVolumeComponent.hpp"

constexpr static const std::int32_t GRID_RADIUS = 16;
constexpr static const CubeCoord GRID_CENTER = {0, 0, 0};

GridGenerationSystem::GridGenerationSystem(ResourceSet *resources)
        : _entityManager(resources->get<EntityManager>()),
          _randomGenerator(resources->get<RandomGenerator>()),
          _sceneManager(resources->get<SceneManager>()) {
    //
}

void GridGenerationSystem::init() {
    auto root = this->_sceneManager->getOrAddRoot("City");

    auto pushCell = [&](const CubeCoord &cube) {
        auto type = this->_randomGenerator->generateInt(0, 3);
        auto entity = this->_entityManager->createEntity();

        auto position = std::make_shared<GridPositionComponent>();
        position->fromCubeCoordinates(cube);

        auto cell = std::make_shared<GridCellComponent>();
        cell->cellType() = static_cast<CellType>(type);
        cell->building() = std::nullopt;

        this->_entityManager->addComponent(entity, position);
        this->_entityManager->addComponent(entity, cell);
        this->_entityManager->addComponent(entity, std::make_shared<RayCollisionVolumeComponent>());

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
