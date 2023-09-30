#ifndef HEXAGON_CITY_BUILDING_COMPONENT_HPP
#define HEXAGON_CITY_BUILDING_COMPONENT_HPP

#include <cstdint>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class BuildingComponent : public Component {
public:
    ~BuildingComponent() override = default;

    [[nodiscard]] std::uint32_t &getColumn() { return this->_column; }

    [[nodiscard]] std::uint32_t &getRow() { return this->_row; }

    [[nodiscard]] std::uint8_t &getLevel() { return this->_level; }

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "Building"; }

private:
    std::uint32_t _column = 0;
    std::uint32_t _row = 0;
    std::uint8_t _level = 1;
};

class BuildingComponentFactory : public Resource, public GenericComponentFactory<BuildingComponent> {
    //
};

#endif // HEXAGON_CITY_BUILDING_COMPONENT_HPP
