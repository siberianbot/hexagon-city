#ifndef HEXAGON_CITY_GRID_POSITION_COMPONENT_HPP
#define HEXAGON_CITY_GRID_POSITION_COMPONENT_HPP

#include <cstdint>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/HexCoordinates.hpp"

using namespace Penrose;

class GridPositionComponent : public Component {
public:
    ~GridPositionComponent() override = default;

    [[nodiscard]] std::int32_t &row() { return std::get<0>(this->_coord); }

    [[nodiscard]] std::int32_t &column() { return std::get<1>(this->_coord); }

    [[nodiscard]] AxialCoord &coord() { return this->_coord; }

    void fromCubeCoordinates(const CubeCoord &cube) {
        this->_coord = convert<CubeCoord, AxialCoord>(cube);
    }

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "GridPosition"; }

private:
    AxialCoord _coord;
};

class GridPositionComponentFactory : public Resource, public GenericComponentFactory<GridPositionComponent> {
    //
};

#endif // HEXAGON_CITY_GRID_POSITION_COMPONENT_HPP
