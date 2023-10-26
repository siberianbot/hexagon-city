#ifndef HEXAGON_CITY_COMPONENTS_HOVERED_COMPONENT_HPP
#define HEXAGON_CITY_COMPONENTS_HOVERED_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

using namespace Penrose;

class HoveredComponent : public Component<HoveredComponent> {
public:
    ~HoveredComponent() override = default;
};

using HoveredComponentFactory = GenericComponentFactory<HoveredComponent>;

#endif // HEXAGON_CITY_COMPONENTS_HOVERED_COMPONENT_HPP
