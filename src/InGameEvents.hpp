#ifndef HEXAGON_CITY_IN_GAME_EVENTS_HPP
#define HEXAGON_CITY_IN_GAME_EVENTS_HPP

#include <cstdint>
#include <optional>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/EventQueue.hpp>

#include "src/BuildingType.hpp"

using namespace Penrose;

class CreateBuildingRequestEvent : public Event<CreateBuildingRequestEvent> {
public:
    CreateBuildingRequestEvent(Entity cell, BuildingType type)
            : _cell(cell),
              _type(type) {
        //
    }

    ~CreateBuildingRequestEvent() override = default;

    [[nodiscard]] const Entity &getCell() const { return this->_cell; }

    [[nodiscard]] const BuildingType &getType() const { return this->_type; }

private:
    Entity _cell;
    BuildingType _type;
};

class UpgradeBuildingRequestEvent : public Event<UpgradeBuildingRequestEvent> {
public:
    explicit UpgradeBuildingRequestEvent(Entity building)
            : _building(building) {
        //
    }

    ~UpgradeBuildingRequestEvent() override = default;

    [[nodiscard]] const Entity &getBuilding() const { return this->_building; }

private:
    Entity _building;
};

class DestroyBuildingRequestEvent : public Event<DestroyBuildingRequestEvent> {
public:
    explicit DestroyBuildingRequestEvent(Entity building)
            : _building(building) {
        //
    }

    ~DestroyBuildingRequestEvent() override = default;

    [[nodiscard]] const Entity &getBuilding() const { return this->_building; }

private:
    Entity _building;
};

class BuildingCreatedEvent : public Event<BuildingCreatedEvent> {
public:
    BuildingCreatedEvent(Entity building, BuildingType type)
            : _building(building),
              _type(type) {
        //
    }

    ~BuildingCreatedEvent() override = default;

    [[nodiscard]] const Entity &getBuilding() const { return this->_building; }

    [[nodiscard]] const BuildingType &getType() const { return this->_type; }

private:
    Entity _building;
    BuildingType _type;
};

class BuildingUpgradedEvent : public Event<BuildingUpgradedEvent> {
public:
    BuildingUpgradedEvent(Entity building, BuildingType type, std::uint8_t level)
            : _building(building),
              _type(type),
              _level(level) {
        //
    }

    ~BuildingUpgradedEvent() override = default;

    [[nodiscard]] const Entity &getBuilding() const { return this->_building; }

    [[nodiscard]] const BuildingType &getType() const { return this->_type; }

    [[nodiscard]] const std::uint8_t &getLevel() const { return this->_level; }

private:
    Entity _building;
    BuildingType _type;
    std::uint8_t _level;
};

class BuildingDestroyedEvent : public Event<BuildingDestroyedEvent> {
public:
    BuildingDestroyedEvent(Entity building, BuildingType type)
            : _building(building),
              _type(type) {
        //
    }

    ~BuildingDestroyedEvent() override = default;

    [[nodiscard]] const Entity &getBuilding() const { return this->_building; }

    [[nodiscard]] const BuildingType &getType() const { return this->_type; }

private:
    Entity _building;
    BuildingType _type;
};

class SelectionChangedEvent : public Event<SelectionChangedEvent> {
public:
    explicit SelectionChangedEvent(std::optional<Entity> selection)
            : _selection(selection) {
        //
    }

    ~SelectionChangedEvent() override = default;

    [[nodiscard]] const std::optional<Entity> &getSelection() const { return this->_selection; }

private:
    std::optional<Entity> _selection;
};

using InGameEventQueue = EventQueue<
        CreateBuildingRequestEvent,
        UpgradeBuildingRequestEvent,
        DestroyBuildingRequestEvent,
        BuildingCreatedEvent,
        BuildingUpgradedEvent,
        BuildingDestroyedEvent,
        SelectionChangedEvent>;

#endif // HEXAGON_CITY_IN_GAME_EVENTS_HPP
