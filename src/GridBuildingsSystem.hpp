#ifndef HEXAGON_CITY_GRID_BUILDINGS_SYSTEM_HPP
#define HEXAGON_CITY_GRID_BUILDINGS_SYSTEM_HPP

#include <map>
#include <memory>
#include <vector>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/BuildingCreateRequestedEvent.hpp"
#include "src/BuildingDestroyRequestedEvent.hpp"
#include "src/BuildingUpgradeRequestedEvent.hpp"
#include "src/GridBuildingComponent.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/PlayerStateContext.hpp"

using namespace Penrose;

class GridBuildingsSystem : public Resource, public Initializable, public System {
public:
    explicit GridBuildingsSystem(ResourceSet *resources);
    ~GridBuildingsSystem() override = default;

    void init() override;
    void destroy() override;

    void update(float) override { /* nothing to do */ }

    [[nodiscard]] std::vector<Entity> getAllNearest(const Entity &center, GridBuildingType type);

    [[nodiscard]] std::string getName() const override { return "GridBuildings"; }

private:
    struct BuildingData {
        std::shared_ptr<GridBuildingComponent> building;
        std::shared_ptr<GridPositionComponent> position;
    };

    using PositionType = std::tuple<AxialCoord, GridBuildingType>;

    Lazy<ECSManager> _ecsManager;
    Lazy<EventQueue> _eventQueue;
    Lazy<PlayerStateContext> _playerStateContext;

    EventQueue::HandlerIdx _eventHandlerIdx = -1;

    std::map<PositionType, Entity> _entitiesPositions;
    std::map<Entity, BuildingData> _entitiesBuildings;

    void handleBuildingCreateRequested(const std::shared_ptr<BuildingCreateRequestedEvent> &event);
    void handleBuildingDestroyRequested(const std::shared_ptr<BuildingDestroyRequestedEvent> &event);
    void handleBuildingUpgradeRequested(const std::shared_ptr<BuildingUpgradeRequestedEvent> &event);
};

#endif // HEXAGON_CITY_GRID_BUILDINGS_SYSTEM_HPP
