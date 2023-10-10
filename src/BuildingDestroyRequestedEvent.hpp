#ifndef HEXAGON_CITY_BUILDING_DESTROY_REQUESTED_EVENT_HPP
#define HEXAGON_CITY_BUILDING_DESTROY_REQUESTED_EVENT_HPP

#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/CustomEvent.hpp>

using namespace Penrose;

class BuildingDestroyRequestedEvent : public CustomEventData {
public:
    explicit BuildingDestroyRequestedEvent(Entity buildingEntity)
            : _buildingEntity(buildingEntity) {
        //
    }

    ~BuildingDestroyRequestedEvent() override = default;

    [[nodiscard]] const Entity &getBuildingEntity() const { return this->_buildingEntity; }

    [[nodiscard]] static std::string name() { return "BuildingDestroyRequested"; }

private:
    Entity _buildingEntity;
};

#endif // HEXAGON_CITY_BUILDING_DESTROY_REQUESTED_EVENT_HPP
