#ifndef HEXAGON_CITY_BUILDING_CREATE_REQUESTED_EVENT_HPP
#define HEXAGON_CITY_BUILDING_CREATE_REQUESTED_EVENT_HPP

#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

#include "src/GridBuildingComponent.hpp"

using namespace Penrose;

class BuildingCreateRequestedEvent : public CustomEventData {
public:
    explicit BuildingCreateRequestedEvent(Entity cellEntity, GridBuildingType buildingType)
            : _cellEntity(cellEntity),
              _buildingType(buildingType) {
        //
    }

    ~BuildingCreateRequestedEvent() override = default;

    [[nodiscard]] const Entity &getCellEntity() const { return this->_cellEntity; }

    [[nodiscard]] const GridBuildingType &getBuildingType() const { return this->_buildingType; }

    [[nodiscard]] static std::string name() { return "BuildingCreateRequested"; }

private:
    Entity _cellEntity;
    GridBuildingType _buildingType;
};

#endif // HEXAGON_CITY_BUILDING_CREATE_REQUESTED_EVENT_HPP
