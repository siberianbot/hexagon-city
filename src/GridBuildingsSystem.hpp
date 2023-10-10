#ifndef HEXAGON_CITY_GRID_BUILDINGS_SYSTEM_HPP
#define HEXAGON_CITY_GRID_BUILDINGS_SYSTEM_HPP

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

using namespace Penrose;

class GridBuildingsSystem : public Resource, public Initializable, public System {
public:
    explicit GridBuildingsSystem(ResourceSet *resources);
    ~GridBuildingsSystem() override = default;

    void init() override;
    void destroy() override;

    void update(float) override;

    [[nodiscard]] std::string getName() const override { return "GridBuildings"; }

private:
    Lazy<ECSManager> _ecsManager;
    Lazy<EventQueue> _eventQueue;

    EventQueue::HandlerIdx _eventHandlerIdx = -1;

    void handleBuildingCreateRequested(const std::shared_ptr<BuildingCreateRequestedEvent> &event);
    void handleBuildingDestroyRequested(const std::shared_ptr<BuildingDestroyRequestedEvent> &event);
    void handleBuildingUpgradeRequested(const std::shared_ptr<BuildingUpgradeRequestedEvent> &event);
};

#endif // HEXAGON_CITY_GRID_BUILDINGS_SYSTEM_HPP
