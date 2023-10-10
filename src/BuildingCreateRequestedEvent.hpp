#ifndef HEXAGON_CITY_BUILDING_CREATE_REQUESTED_EVENT_HPP
#define HEXAGON_CITY_BUILDING_CREATE_REQUESTED_EVENT_HPP

#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

using namespace Penrose;

class BuildingCreateRequestedEvent : public CustomEventData {
public:
    explicit BuildingCreateRequestedEvent(Entity cellEntity)
            : _cellEntity(cellEntity) {
        //
    }

    ~BuildingCreateRequestedEvent() override = default;

    [[nodiscard]] const Entity &getCellEntity() const { return this->_cellEntity; }

    [[nodiscard]] static std::string name() { return "BuildingCreateRequested"; }

private:
    Entity _cellEntity;
};

#endif // HEXAGON_CITY_BUILDING_CREATE_REQUESTED_EVENT_HPP
