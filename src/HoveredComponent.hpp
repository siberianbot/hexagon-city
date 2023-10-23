#ifndef HEXAGON_CITY_HOVERED_COMPONENT_HPP
#define HEXAGON_CITY_HOVERED_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class HoveredComponent : public Component {
public:
    ~HoveredComponent() override = default;

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "Hovered"; }
};

class HoveredComponentFactory : public Resource<HoveredComponentFactory>,
                                public GenericComponentFactory<HoveredComponent> {
    //
};

#endif // HEXAGON_CITY_HOVERED_COMPONENT_HPP
