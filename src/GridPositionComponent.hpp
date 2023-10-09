#ifndef HEXAGON_CITY_GRID_POSITION_COMPONENT_HPP
#define HEXAGON_CITY_GRID_POSITION_COMPONENT_HPP

#include <cstdint>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class GridPositionComponent : public Component {
public:
    ~GridPositionComponent() override = default;

    [[nodiscard]] std::int32_t &row() { return this->_row; }

    [[nodiscard]] std::int32_t &column() { return this->_column; }

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "GridPosition"; }

private:
    std::int32_t _row;
    std::int32_t _column;
};

class GridPositionComponentFactory : public Resource, public GenericComponentFactory<GridPositionComponent> {
    //
};

#endif // HEXAGON_CITY_GRID_POSITION_COMPONENT_HPP
