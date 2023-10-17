#include "GridBuildingsSystem.hpp"

#include "src/BuildingCreatedEvent.hpp"
#include "src/BuildingDestroyedEvent.hpp"
#include "src/BuildingUpgradedEvent.hpp"
#include "src/GridBuildingComponent.hpp"
#include "src/GridCellComponent.hpp"
#include "src/GridConstants.hpp"
#include "src/GridPositionComponent.hpp"
#include "src/RayCollisionVolumeComponent.hpp"

GridBuildingsSystem::GridBuildingsSystem(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()),
          _eventQueue(resources->getLazy<EventQueue>()),
          _playerStateContext(resources->getLazy<PlayerStateContext>()) {
    //
}

void GridBuildingsSystem::init() {
    this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::CustomEvent, CustomEventArgs>(
            [this](const CustomEvent *event) {
                if (event->getArgs().type == BuildingCreateRequestedEvent::name()) {
                    this->handleBuildingCreateRequested(
                            std::dynamic_pointer_cast<BuildingCreateRequestedEvent>(event->getArgs().data)
                    );
                }

                if (event->getArgs().type == BuildingDestroyRequestedEvent::name()) {
                    this->handleBuildingDestroyRequested(
                            std::dynamic_pointer_cast<BuildingDestroyRequestedEvent>(event->getArgs().data)
                    );
                }

                if (event->getArgs().type == BuildingUpgradeRequestedEvent::name()) {
                    this->handleBuildingUpgradeRequested(
                            std::dynamic_pointer_cast<BuildingUpgradeRequestedEvent>(event->getArgs().data)
                    );
                }
            });
}

void GridBuildingsSystem::destroy() {
    this->_eventQueue->removeHandler(this->_eventHandlerIdx);
}

void GridBuildingsSystem::update(float) {
    // TODO?
}

void GridBuildingsSystem::handleBuildingCreateRequested(const std::shared_ptr<BuildingCreateRequestedEvent> &event) {

    auto cellPosition = this->_ecsManager->getComponent<GridPositionComponent>(event->getCellEntity());
    auto cell = this->_ecsManager->getComponent<GridCellComponent>(event->getCellEntity());

    if (cell->type() == GridCellType::Water) {
        // TODO notify "Can't build on water"

        return;
    }

    if (cell->building().has_value()) {
        // TODO notify "There is a building already"

        return;
    }

    if (this->_playerStateContext->balance() < GRID_BUILDING_COST) {
        // TODO notify "Not enough money"

        return;
    }

    auto buildingEntity = this->_ecsManager->createEntity();

    auto buildingPosition = this->_ecsManager->addComponent<GridPositionComponent>(buildingEntity);
    buildingPosition->row() = cellPosition->row();
    buildingPosition->column() = cellPosition->column();

    auto building = this->_ecsManager->addComponent<GridBuildingComponent>(buildingEntity);
    building->level() = 1;
    building->type() = event->getBuildingType();
    building->cell() = event->getCellEntity();

    this->_ecsManager->addComponent<RayCollisionVolumeComponent>(buildingEntity);

    cell->building() = buildingEntity;

    this->_playerStateContext->balance() -= GRID_BUILDING_COST;

    this->_eventQueue->pushEvent<EventType::CustomEvent>(
            makeCustomEventArgs(new BuildingCreatedEvent(buildingEntity, building->type()))
    );
}

void GridBuildingsSystem::handleBuildingDestroyRequested(const std::shared_ptr<BuildingDestroyRequestedEvent> &event) {

    auto building = this->_ecsManager->getComponent<GridBuildingComponent>(event->getBuildingEntity());

    this->_ecsManager->getComponent<GridCellComponent>(building->cell())->building() = std::nullopt;

    auto entity = event->getBuildingEntity();
    this->_ecsManager->destroyEntity(std::forward<decltype(entity)>(entity));

    this->_eventQueue->pushEvent<EventType::CustomEvent>(
            makeCustomEventArgs(new BuildingDestroyedEvent(event->getBuildingEntity(), building->type()))
    );
}

void GridBuildingsSystem::handleBuildingUpgradeRequested(const std::shared_ptr<BuildingUpgradeRequestedEvent> &event) {

    auto building = this->_ecsManager->getComponent<GridBuildingComponent>(event->getBuildingEntity());

    if (building->level() >= GRID_BUILDING_MAX_LEVEL) {
        // TODO notify "Building already upgrade to its max level"

        return;
    }

    if (this->_playerStateContext->balance() < GRID_BUILDING_UPGRADE_COST) {
        // TODO notify "Not enough money"

        return;
    }

    building->level()++;

    this->_playerStateContext->balance() -= GRID_BUILDING_UPGRADE_COST;

    this->_eventQueue->pushEvent<EventType::CustomEvent>(
            makeCustomEventArgs(new BuildingUpgradedEvent(event->getBuildingEntity(),
                                                          building->type(),
                                                          building->level()))
    );
}
