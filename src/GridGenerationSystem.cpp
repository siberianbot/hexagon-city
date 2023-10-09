#include "GridGenerationSystem.hpp"

#include <random>

#include "src/GridCellComponent.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/RayCollisionVolumeComponent.hpp"

constexpr static const std::int32_t GRID_WIDTH = 16;
constexpr static const std::int32_t GRID_HEIGHT = 16;

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

    for (std::int32_t row = 0; row < GRID_WIDTH; row++) {
        for (std::int32_t column = 0; column < GRID_HEIGHT; column++) {

            auto type = distribution(randomEngine);
            auto entity = this->_ecsManager->createEntity();

            auto position = this->_ecsManager->addComponent<GridPositionComponent>(entity);
            position->row() = row;
            position->column() = column;

            auto cell = this->_ecsManager->addComponent<GridCellComponent>(entity);
            cell->type() = static_cast<GridCellType>(type);
            cell->building() = std::nullopt;

            this->_ecsManager->addComponent<RayCollisionVolumeComponent>(entity);

            this->_sceneManager->insertEntityNode(root, entity);
        }
    }
}
