#ifndef HEXAGON_CITY_BUILDING_UPGRADE_REQUESTED_EVENT_HPP
#define HEXAGON_CITY_BUILDING_UPGRADE_REQUESTED_EVENT_HPP

#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

using namespace Penrose;

class BuildingUpgradeRequestedEvent : public CustomEventData {
public:
    explicit BuildingUpgradeRequestedEvent(Entity buildingEntity)
            : _buildingEntity(buildingEntity) {
        //
    }

    ~BuildingUpgradeRequestedEvent() override = default;

    [[nodiscard]] const Entity &getBuildingEntity() const { return this->_buildingEntity; }

    [[nodiscard]] static std::string name() { return "BuildingUpgradeRequested"; }

private:
    Entity _buildingEntity;
};

#endif // HEXAGON_CITY_BUILDING_UPGRADE_REQUESTED_EVENT_HPP
