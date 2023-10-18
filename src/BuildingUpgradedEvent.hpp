#ifndef HEXAGON_CITY_BUILDING_UPGRADED_EVENT_HPP
#define HEXAGON_CITY_BUILDING_UPGRADED_EVENT_HPP

#include <cstdint>
#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

#include "src/GridBuildingComponent.hpp"

using namespace Penrose;

class BuildingUpgradedEvent : public CustomEventData {
public:
    explicit BuildingUpgradedEvent(Entity buildingEntity, GridBuildingType buildingType, std::uint8_t newLevel)
            : _buildingEntity(buildingEntity),
              _buildingType(buildingType),
              _newLevel(newLevel) {
        //
    }

    ~BuildingUpgradedEvent() override = default;

    [[nodiscard]] const Entity &getBuildingEntity() const { return this->_buildingEntity; }

    [[nodiscard]] const GridBuildingType &getBuildingType() const { return this->_buildingType; }

    [[nodiscard]] const std::uint8_t &getNewLevel() const { return this->_newLevel; }

    [[nodiscard]] static std::string name() { return "BuildingUpgraded"; }

private:
    Entity _buildingEntity;
    GridBuildingType _buildingType;
    std::uint8_t _newLevel;
};

#endif // HEXAGON_CITY_BUILDING_UPGRADED_EVENT_HPP
