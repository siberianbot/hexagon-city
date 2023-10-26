#include "GridBuildingsSystem.hpp"

#include <stdexcept>

#include "src/GridConstants.hpp"
#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridCellComponent.hpp"
#include "src/Components/GridPositionComponent.hpp"
#include "src/Components/RayCollisionVolumeComponent.hpp"

GridBuildingsSystem::GridBuildingsSystem(ResourceSet *resources)
        : _entityManager(resources->get<EntityManager>()),
          _inGameEventQueue(resources->get<InGameEventQueue>()),
          _playerStateContext(resources->get<PlayerStateContext>()) {
    //
}

void GridBuildingsSystem::init() {
    this->_inGameEventQueue->addHandler<CreateBuildingRequestEvent>(
            [this](const CreateBuildingRequestEvent *event) {
                this->handleCreateBuildingRequest(event);
            });

    this->_inGameEventQueue->addHandler<UpgradeBuildingRequestEvent>(
            [this](const UpgradeBuildingRequestEvent *event) {
                this->handleUpgradeBuildingRequest(event);
            });

    this->_inGameEventQueue->addHandler<DestroyBuildingRequestEvent>(
            [this](const DestroyBuildingRequestEvent *event) {
                this->handleDestroyBuildingRequest(event);
            });
}

void GridBuildingsSystem::destroy() {
    this->_entitiesBuildings.clear();
    this->_entitiesPositions.clear();
}

std::vector<Entity> GridBuildingsSystem::getAllNearest(const Entity &center, BuildingType type) {
    auto data = orElseThrow(
            tryGetPointer(this->_entitiesBuildings, center),
            std::logic_error("No such building entity")
    );

    std::vector<Entity> entities;

    for (std::uint32_t dir = 0; dir < AXIAL_DIRECTIONS.size(); dir++) {
        auto positionType = std::make_tuple(axialNeighbor(data->position->coord(), dir), type);
        auto maybeEntity = tryGet(this->_entitiesPositions, positionType);

        if (maybeEntity.has_value()) {
            entities.emplace_back(*maybeEntity);
        }
    }

    return entities;
}

void GridBuildingsSystem::handleCreateBuildingRequest(const CreateBuildingRequestEvent *event) {

    auto cellPosition = this->_entityManager->getComponent<GridPositionComponent>(event->getCell());
    auto cell = this->_entityManager->getComponent<GridCellComponent>(event->getCell());

    if (cell->cellType() == CellType::Water) {
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

    auto buildingEntity = this->_entityManager->createEntity();

    auto buildingPosition = std::make_shared<GridPositionComponent>();
    buildingPosition->coord() = cellPosition->coord();

    auto building = std::make_shared<GridBuildingComponent>();
    building->level() = 1;
    building->cell() = event->getCell();
    building->buildingType() = event->getType();

    this->_entityManager->addComponent(buildingEntity, buildingPosition);
    this->_entityManager->addComponent(buildingEntity, building);
    this->_entityManager->addComponent(buildingEntity, std::make_shared<RayCollisionVolumeComponent>());

    cell->building() = buildingEntity;

    this->_playerStateContext->balance() -= GRID_BUILDING_COST;

    this->_entitiesPositions.emplace(std::make_tuple(buildingPosition->coord(), building->buildingType()),
                                     buildingEntity);
    this->_entitiesBuildings.emplace(buildingEntity, BuildingData{
            .building = building,
            .position = buildingPosition
    });

    this->_inGameEventQueue->push<BuildingCreatedEvent>(buildingEntity, building->buildingType());
}

void GridBuildingsSystem::handleUpgradeBuildingRequest(const UpgradeBuildingRequestEvent *event) {

    auto building = this->_entityManager->getComponent<GridBuildingComponent>(event->getBuilding());

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

    this->_inGameEventQueue->push<BuildingUpgradedEvent>(event->getBuilding(),
                                                         building->buildingType(),
                                                         building->level());
}

void GridBuildingsSystem::handleDestroyBuildingRequest(const DestroyBuildingRequestEvent *event) {

    auto building = this->_entityManager->getComponent<GridBuildingComponent>(event->getBuilding());

    this->_entityManager->getComponent<GridCellComponent>(building->cell())->building() = std::nullopt;

    auto coord = this->_entityManager->getComponent<GridPositionComponent>(event->getBuilding())->coord();
    this->_entitiesPositions.erase(std::make_tuple(coord, building->buildingType()));
    this->_entitiesBuildings.erase(event->getBuilding());

    this->_entityManager->destroyEntity(event->getBuilding());

    this->_inGameEventQueue->push<BuildingDestroyedEvent>(event->getBuilding(), building->buildingType());
}
