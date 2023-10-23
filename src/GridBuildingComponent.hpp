#ifndef HEXAGON_CITY_GRID_BUILDING_COMPONENT_HPP
#define HEXAGON_CITY_GRID_BUILDING_COMPONENT_HPP

#include <cstdint>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

enum class GridBuildingType : std::uint32_t {
    Residential,
    Industrial,
    Commercial
};

class GridBuildingComponent : public Component {
public:
    ~GridBuildingComponent() override = default;

    [[nodiscard]] GridBuildingType &type() { return this->_type; }

    [[nodiscard]] std::uint8_t &level() { return this->_level; }

    [[nodiscard]] Entity &cell() { return this->_cell; }

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "GridBuilding"; }

private:
    GridBuildingType _type;
    std::uint8_t _level;
    Entity _cell;
};

class GridBuildingComponentFactory : public Resource<GridBuildingComponentFactory>,
                                     public GenericComponentFactory<GridBuildingComponent> {
    //
};

#endif // HEXAGON_CITY_GRID_BUILDING_COMPONENT_HPP
