#ifndef HEXAGON_CITY_COMPONENTS_SELECTED_COMPONENT_HPP
#define HEXAGON_CITY_COMPONENTS_SELECTED_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

using namespace Penrose;

class SelectedComponent : public Component<SelectedComponent> {
public:
    ~SelectedComponent() override = default;
};

using SelectedComponentFactory = GenericComponentFactory<SelectedComponent>;

#endif // HEXAGON_CITY_COMPONENTS_SELECTED_COMPONENT_HPP
