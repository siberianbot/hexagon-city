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

    [[nodiscard]] GridCellType &type() { return this->_type; }

    [[nodiscard]] std::optional<Entity> &building() { return this->_building; }

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "GridCell"; }

private:
    GridCellType _type;
    std::optional<Entity> _building;
};

class GridCellComponentFactory : public Resource, public GenericComponentFactory<GridCellComponent> {
    //
};

#endif // HEXAGON_CITY_GRID_CELL_COMPONENT_HPP
