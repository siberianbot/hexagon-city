#include "DebugCityGenerationSystem.hpp"

#include <random>
#include <set>
#include <tuple>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

#include "src/BulidingComponent.hpp"
#include "src/RayCollisionVolumeComponent.hpp"

DebugCityGenerationSystem::DebugCityGenerationSystem(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()),
          _sceneManager(resources->getLazy<SceneManager>()) {
    //
}

void DebugCityGenerationSystem::init() {
    auto root = this->_sceneManager->getOrAddRoot("City");

    auto randomDevice = std::random_device();
    auto randomEngine = std::default_random_engine(randomDevice());
    auto distribution = std::uniform_int_distribution(-3, 3);

    std::set<std::tuple<int, int>> placed;

    for (std::uint32_t n = 0; n < 16; n++) {

        int row;
        int column;
        std::tuple<int, int> place;

        do {
            row = distribution(randomEngine);
            column = distribution(randomEngine);

            place = std::make_tuple(row, column);
        } while (placed.contains(place));

        placed.insert(place);

        auto entity = this->_ecsManager->createEntity();

        auto transform = this->_ecsManager->addComponent<TransformComponent>(entity);
        transform->getPos().x = static_cast<float>(3. / 2 * column);
        transform->getPos().z = static_cast<float>(sqrt(3) / 2 * column + sqrt(3) * row);
        transform->getScale() *= 0.75;

        auto meshRenderer = this->_ecsManager->addComponent<MeshRendererComponent>(entity);
        meshRenderer->setMeshAsset("models/building.asset");
        meshRenderer->setAlbedoTextureAsset("textures/building.asset");

        auto building = this->_ecsManager->addComponent<BuildingComponent>(entity);
        building->getRow() = row;
        building->getColumn() = column;
        building->getLevel() = 1;

        this->_ecsManager->addComponent<RayCollisionVolumeComponent>(entity);

        this->_sceneManager->insertEntityNode(root, entity);
    }
}
