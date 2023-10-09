#ifndef HEXAGON_CITY_SELECTION_CHANGED_EVENT_HPP
#define HEXAGON_CITY_SELECTION_CHANGED_EVENT_HPP

#include <optional>
#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

using namespace Penrose;

class SelectionChangedEvent : public CustomEventData {
public:
    explicit SelectionChangedEvent(std::optional<Entity> newSelection)
            : _newSelection(newSelection) {
        //
    }

    ~SelectionChangedEvent() override = default;

    [[nodiscard]] const std::optional<Entity> &getNewSelection() const { return this->_newSelection; }

    [[nodiscard]] static std::string name() { return "SelectionChanged"; }

private:
    std::optional<Entity> _newSelection;
};

#endif // HEXAGON_CITY_SELECTION_CHANGED_EVENT_HPP
