#include "GridGenerationSystem.hpp"

#include <cstdint>
#include <cmath>
#include <random>

#include <glm/vec3.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

#include "src/GridCellComponent.hpp"
#include "src/GridConstants.hpp"
#include "src/RayCollisionVolumeComponent.hpp"

constexpr static const std::uint32_t GRID_WIDTH = 16;
constexpr static const std::uint32_t GRID_HEIGHT = 16;

constexpr static const float GRID_CELL_RADIUS = 0.5f;
constexpr static const float GRID_CELL_DIAMETER = 2 * GRID_CELL_RADIUS;

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

    for (std::uint32_t row = 0; row < GRID_WIDTH; row++) {
        for (std::uint32_t column = 0; column < GRID_HEIGHT; column++) {

            auto type = distribution(randomEngine);

            auto entity = this->_ecsManager->createEntity();

            auto transform = this->_ecsManager->addComponent<TransformComponent>(entity);
            transform->getPos() = GRID_CELL_DIAMETER * glm::vec3(sqrt(3) * column + sqrt(3) / 2 * row,
                                                                 0,
                                                                 3. / 2 * row);
            transform->getScale() = glm::vec3(1, 0.1, 1);

            auto meshRenderer = this->_ecsManager->addComponent<MeshRendererComponent>(entity);
            meshRenderer->setMeshAsset("models/building.asset");
            meshRenderer->setAlbedoTextureAsset("textures/building.asset");
            meshRenderer->setColor(GRID_CELL_COLORS[type]);

            auto gridCell = this->_ecsManager->addComponent<GridCellComponent>(entity);
            gridCell->getRow() = row;
            gridCell->getColumn() = column;
            gridCell->getType() = static_cast<GridCellType>(type);
            gridCell->getBuilding() = std::nullopt;

            this->_ecsManager->addComponent<RayCollisionVolumeComponent>(entity);

            this->_sceneManager->insertEntityNode(root, entity);
        }
    }
}
