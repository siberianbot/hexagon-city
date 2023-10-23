#ifndef HEXAGON_CITY_SELECTED_COMPONENT_HPP
#define HEXAGON_CITY_SELECTED_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class SelectedComponent : public Component {
public:
    ~SelectedComponent() override = default;

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "Selected"; }
};

class SelectedComponentFactory : public Resource<SelectedComponentFactory>,
                                 public GenericComponentFactory<SelectedComponent> {
    //
};

#endif // HEXAGON_CITY_SELECTED_COMPONENT_HPP
