#ifndef HEXAGON_CITY_SYSTEMS_GRID_BUILDINGS_SYSTEM_HPP
#define HEXAGON_CITY_SYSTEMS_GRID_BUILDINGS_SYSTEM_HPP

#include <map>
#include <memory>
#include <vector>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/InGameEvents.hpp"
#include "src/PlayerStateContext.hpp"
#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridPositionComponent.hpp"

using namespace Penrose;

class GridBuildingsSystem : public Resource<GridBuildingsSystem, ResourceGroup::ECSSystem>,
                            public Initializable,
                            public System {
public:
    explicit GridBuildingsSystem(ResourceSet *resources);
    ~GridBuildingsSystem() override = default;

    void init() override;
    void destroy() override;

    void update(float) override { /* nothing to do */ }

    [[nodiscard]] std::vector<Entity> getAllNearest(const Entity &center, BuildingType type);

    [[nodiscard]] std::string getName() const override { return "GridBuildings"; }

private:
    struct BuildingData {
        std::shared_ptr<GridBuildingComponent> building;
        std::shared_ptr<GridPositionComponent> position;
    };

    using PositionType = std::tuple<AxialCoord, BuildingType>;

    ResourceProxy<EntityManager> _entityManager;
    ResourceProxy<InGameEventQueue> _inGameEventQueue;
    ResourceProxy<PlayerStateContext> _playerStateContext;

    std::map<PositionType, Entity> _entitiesPositions;
    std::map<Entity, BuildingData> _entitiesBuildings;

    void handleCreateBuildingRequest(const CreateBuildingRequestEvent *event);
    void handleUpgradeBuildingRequest(const UpgradeBuildingRequestEvent *event);
    void handleDestroyBuildingRequest(const DestroyBuildingRequestEvent *event);
};

#endif // HEXAGON_CITY_SYSTEMS_GRID_BUILDINGS_SYSTEM_HPP
