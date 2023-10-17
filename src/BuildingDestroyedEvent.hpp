#ifndef HEXAGON_CITY_BUILDING_DESTROYED_EVENT_HPP
#define HEXAGON_CITY_BUILDING_DESTROYED_EVENT_HPP

#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

#include "src/GridBuildingComponent.hpp"

using namespace Penrose;

class BuildingDestroyedEvent : public CustomEventData {
public:
    explicit BuildingDestroyedEvent(Entity buildingEntity, GridBuildingType buildingType)
            : _buildingEntity(buildingEntity),
              _buildingType(buildingType) {
        //
    }

    ~BuildingDestroyedEvent() override = default;

    [[nodiscard]] const Entity &getBuildingEntity() const { return this->_buildingEntity; }

    [[nodiscard]] const GridBuildingType &getBuildingType() const { return this->_buildingType; }

    [[nodiscard]] static std::string name() { return "BuildingDestroyed"; }

private:
    Entity _buildingEntity;
    GridBuildingType _buildingType;
};

#endif // HEXAGON_CITY_BUILDING_DESTROYED_EVENT_HPP
