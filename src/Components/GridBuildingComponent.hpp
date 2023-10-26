#ifndef HEXAGON_CITY_COMPONENTS_GRID_BUILDING_COMPONENT_HPP
#define HEXAGON_CITY_COMPONENTS_GRID_BUILDING_COMPONENT_HPP

#include <cstdint>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/ECS/Entity.hpp>

#include "src/BuildingType.hpp"

using namespace Penrose;

class GridBuildingComponent : public Component<GridBuildingComponent> {
public:
    ~GridBuildingComponent() override = default;

    [[nodiscard]] BuildingType &buildingType() { return this->_type; }

    [[nodiscard]] std::uint8_t &level() { return this->_level; }

    [[nodiscard]] Entity &cell() { return this->_cell; }

private:
    BuildingType _type;
    std::uint8_t _level;
    Entity _cell;
};

using GridBuildingComponentFactory = GenericComponentFactory<GridBuildingComponent>;

#endif // HEXAGON_CITY_COMPONENTS_GRID_BUILDING_COMPONENT_HPP
