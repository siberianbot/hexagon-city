#ifndef HEXAGON_CITY_BUILDING_COMPONENT_HPP
#define HEXAGON_CITY_BUILDING_COMPONENT_HPP

#include <cstdint>
#include <string_view>

#include <Penrose/ECS/Component.hpp>

class BuildingComponent : public Penrose::Component {
public:
    ~BuildingComponent() override = default;

    [[nodiscard]] std::uint32_t &getColumn() { return this->_column; }

    [[nodiscard]] std::uint32_t &getRow() { return this->_row; }

    [[nodiscard]] std::uint8_t &getLevel() { return this->_level; }

    [[nodiscard]] static std::string_view name() { return "Building"; }

private:
    std::uint32_t _column = 0;
    std::uint32_t _row = 0;
    std::uint8_t _level = 1;
};

#endif // HEXAGON_CITY_BUILDING_COMPONENT_HPP
