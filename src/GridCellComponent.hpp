#ifndef HEXAGON_CITY_GRID_CELL_COMPONENT_HPP
#define HEXAGON_CITY_GRID_CELL_COMPONENT_HPP

#include <cstdint>
#include <optional>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

enum class GridCellType {
    Concrete,
    Grass,
    Sand,
    Water
};

class GridCellComponent : public Component {
public:
    ~GridCellComponent() override = default;

    [[nodiscard]] std::uint32_t &getColumn() { return this->_column; }

    [[nodiscard]] std::uint32_t &getRow() { return this->_row; }

    [[nodiscard]] GridCellType &getType() { return this->_type; }

    [[nodiscard]] std::optional<Entity> &getBuilding() { return this->_building; }

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "GridCell"; }

private:
    std::uint32_t _column = 0;
    std::uint32_t _row = 0;
    GridCellType _type;
    std::optional<Entity> _building;
};

class GridCellComponentFactory : public Resource, public GenericComponentFactory<GridCellComponent> {
    //
};

#endif // HEXAGON_CITY_GRID_CELL_COMPONENT_HPP
