#ifndef HEXAGON_CITY_SYSTEMS_GRID_GENERATION_SYSTEM_HPP
#define HEXAGON_CITY_SYSTEMS_GRID_GENERATION_SYSTEM_HPP

#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include "src/HexCoordinates.hpp"
#include "src/RandomGenerator.hpp"

using namespace Penrose;

class GridGenerationSystem : public Resource<GridGenerationSystem, ResourceGroup::ECSSystem>,
                             public Initializable,
                             public System {
public:
    explicit GridGenerationSystem(ResourceSet *resources);
    ~GridGenerationSystem() override = default;

    void init() override;

    void destroy() override { /* nothing to do */ }

    void update(float) override { /* nothing to do */ }

    [[nodiscard]] std::string getName() const override { return "GridGeneration"; }

private:
    ResourceProxy<EntityManager> _entityManager;
    ResourceProxy<RandomGenerator> _randomGenerator;
    ResourceProxy<SceneManager> _sceneManager;
};

#endif // HEXAGON_CITY_SYSTEMS_GRID_GENERATION_SYSTEM_HPP
