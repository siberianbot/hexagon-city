#ifndef HEXAGON_CITY_COMPONENTS_GRID_POSITION_COMPONENT_HPP
#define HEXAGON_CITY_COMPONENTS_GRID_POSITION_COMPONENT_HPP

#include <cstdint>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

#include "src/HexCoordinates.hpp"

using namespace Penrose;

class GridPositionComponent : public Component<GridPositionComponent> {
public:
    ~GridPositionComponent() override = default;

    [[nodiscard]] std::int32_t &row() { return std::get<0>(this->_coord); }

    [[nodiscard]] std::int32_t &column() { return std::get<1>(this->_coord); }

    [[nodiscard]] AxialCoord &coord() { return this->_coord; }

    void fromCubeCoordinates(const CubeCoord &cube) {
        this->_coord = convert<CubeCoord, AxialCoord>(cube);
    }

private:
    AxialCoord _coord;
};

using GridPositionComponentFactory = GenericComponentFactory<GridPositionComponent>;

#endif // HEXAGON_CITY_COMPONENTS_GRID_POSITION_COMPONENT_HPP
