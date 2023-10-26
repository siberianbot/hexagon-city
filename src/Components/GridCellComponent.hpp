#ifndef HEXAGON_CITY_COMPONENTS_GRID_CELL_COMPONENT_HPP
#define HEXAGON_CITY_COMPONENTS_GRID_CELL_COMPONENT_HPP

#include <cstdint>
#include <optional>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/ECS/Entity.hpp>

#include "src/CellType.hpp"

using namespace Penrose;

class GridCellComponent : public Component<GridCellComponent> {
public:
    ~GridCellComponent() override = default;

    [[nodiscard]] CellType &cellType() { return this->_type; }

    [[nodiscard]] std::optional<Entity> &building() { return this->_building; }

private:
    CellType _type;
    std::optional<Entity> _building;
};

using GridCellComponentFactory = GenericComponentFactory<GridCellComponent>;

#endif // HEXAGON_CITY_COMPONENTS_GRID_CELL_COMPONENT_HPP
