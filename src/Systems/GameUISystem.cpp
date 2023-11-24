#include "GameUISystem.hpp"

#include <fmt/core.h>

GameUISystem::GameUISystem(ResourceSet *resources)
    : _entityManager(resources->get<EntityManager>()),
      _inGameEventQueue(resources->get<InGameEventQueue>()),
      _uiManager(resources->get<UIManager>()),
      _citySimulationSystem(resources->get<CitySimulationSystem>()),
      _playerStateContext(resources->get<PlayerStateContext>()) {
    //
}

void GameUISystem::init() {

    auto selectionOpened = std::make_shared<BooleanValue>(false);
    auto selectedBuildingType = std::make_shared<IntegerValue>(static_cast<int>(BuildingType::Residential));

    this->_rootContext = std::make_shared<ObjectValue>(
        ObjectValue()
            .property<ObjectValue>(
                "simulation",
                ObjectValue().property<StringValue>(
                    "population_income_timer",
                    [this]() {
                        return fmt::format(
                            "Population income timer: {}",
                            this->_citySimulationSystem->getPopulationIncomeTimer()
                        );
                    },
                    [](std::string) {}
                )
            )
            .property<ObjectValue>(
                "player",
                ObjectValue().property<StringValue>(
                    "balance",
                    [this]() { return fmt::format("Balance: {}", this->_playerStateContext->balance()); },
                    [](std::string) {}
                )
            )
            .property("selection_opened", selectionOpened)
            .property<ObjectValue>(
                "selection",
                ObjectValue()
                    .property<BooleanValue>(
                        "is_cell",
                        [this]() { return this->_selection.has_value() && this->_selection->cell.has_value(); },
                        [](bool) {}
                    )
                    .property<ListValue<StringValue>>(
                        "building_types",
                        ListValue<StringValue>().push("Residential").push("Industrial").push("Commercial")
                    )
                    .property("selected_building_type", selectedBuildingType)
                    .property<ActionValue>(
                        "create_building",
                        [this, selectedBuildingType]() {
                            if (!this->_selection.has_value()) {
                                return;
                            }

                            this->_inGameEventQueue->push<CreateBuildingRequestEvent>(
                                this->_selection->entity,
                                static_cast<BuildingType>(selectedBuildingType->getValue())
                            );
                        }
                    )
                    .property<BooleanValue>(
                        "is_building",
                        [this]() { return this->_selection.has_value() && this->_selection->building.has_value(); },
                        [](bool) {}
                    )
                    .property<ActionValue>(
                        "upgrade_building",
                        [this]() {
                            if (!this->_selection.has_value()) {
                                return;
                            }

                            this->_inGameEventQueue->push<UpgradeBuildingRequestEvent>(this->_selection->entity);
                        }
                    )
                    .property<ActionValue>(
                        "destroy_building",
                        [this]() {
                            if (!this->_selection.has_value()) {
                                return;
                            }

                            this->_inGameEventQueue->push<DestroyBuildingRequestEvent>(this->_selection->entity);
                            this->_selection = std::nullopt;
                        }
                    )
                    .property<BooleanValue>(
                        "is_residential",
                        [this]() { return this->_selection.has_value() && this->_selection->residential.has_value(); },
                        [](bool) {}
                    )
                    .property<ObjectValue>(
                        "residential",
                        ObjectValue()
                            .property<StringValue>(
                                "count",
                                [this]() { return fmt::format("Count: {}", (*this->_selection->residential)->count); },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "capacity",
                                [this]() {
                                    return fmt::format("Capacity: {}", (*this->_selection->residential)->capacity);
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "groups_count",
                                [this]() {
                                    return fmt::format(
                                        "Count of groups: {}",
                                        (*this->_selection->residential)->residents.size()
                                    );
                                },
                                [](std::string) {}
                            )
                            .property<ListValue<ObjectValue>>("groups")
                            .property<IntegerValue>("selected_group", -1)
                    )
                    .property<BooleanValue>(
                        "is_industrial",
                        [this]() { return this->_selection.has_value() && this->_selection->industrial.has_value(); },
                        [](bool) {}
                    )
                    .property<ObjectValue>(
                        "industrial",
                        ObjectValue()
                            .property<StringValue>(
                                "employees_capacity",
                                [this]() {
                                    return fmt::format(
                                        "Employees Capacity: {}",
                                        (*this->_selection->industrial)->employeesCapacity
                                    );
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "products",
                                [this]() {
                                    return fmt::format("Products: {}", (*this->_selection->industrial)->products);
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "product_capacity",
                                [this]() {
                                    return fmt::format(
                                        "Product Capacity: {}",
                                        (*this->_selection->industrial)->productCapacity
                                    );
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "product_multiplier",
                                [this]() {
                                    return fmt::format(
                                        "Product Multiplier: {}",
                                        (*this->_selection->industrial)->productMultiplier
                                    );
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "query_timer",
                                [this]() {
                                    return fmt::format("Query Timer: {}", (*this->_selection->industrial)->queryTimer);
                                },
                                [](std::string) {}
                            )
                    )
                    .property<BooleanValue>(
                        "is_commercial",
                        [this]() { return this->_selection.has_value() && this->_selection->commercial.has_value(); },
                        [](bool) {}
                    )
                    .property<ObjectValue>(
                        "commercial",
                        ObjectValue()
                            .property<StringValue>(
                                "happiness",
                                [this]() {
                                    return fmt::format("Happiness: {}", (*this->_selection->commercial)->happiness);
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "happiness_capacity",
                                [this]() {
                                    return fmt::format(
                                        "Happiness Capacity: {}",
                                        (*this->_selection->commercial)->happinessCapacity
                                    );
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "happiness_multiplier",
                                [this]() {
                                    return fmt::format(
                                        "Happiness Multiplier: {}",
                                        (*this->_selection->commercial)->happinessMultiplier
                                    );
                                },
                                [](std::string) {}
                            )
                            .property<StringValue>(
                                "query_timer",
                                [this]() {
                                    return fmt::format("Query Timer: {}", (*this->_selection->commercial)->queryTimer);
                                },
                                [](std::string) {}
                            )
                    )
            )
    );

    this->_uiManager->createContext("GameUI");

    this->_inGameEventQueue->addHandler<SelectionChangedEvent>([this,
                                                                selectionOpened](const SelectionChangedEvent *event) {
        selectionOpened->setValue(event->getSelection().has_value());

        if (event->getSelection().has_value()) {
            const auto entity = *event->getSelection();

            this->_selection = SelectionData {
                .entity = entity,
                .cell = this->_entityManager->tryGetComponent<GridCellComponent>(entity),
                .building = this->_entityManager->tryGetComponent<GridBuildingComponent>(entity),
                .residential = tryGetPointer(this->_citySimulationSystem->getResidentials(), entity),
                .industrial = tryGetPointer(this->_citySimulationSystem->getIndustrials(), entity),
                .commercial = tryGetPointer(this->_citySimulationSystem->getCommercials(), entity)
            };
        } else {
            this->_selection = std::nullopt;
        }
    });
}

void GameUISystem::destroy() {
    this->_uiManager->destroyContext("GameUI");
}

void GameUISystem::run() {
    this->_uiManager->addLayoutToContext("GameUI", "layouts/selection.asset", std::shared_ptr(this->_rootContext));
    this->_uiManager->addLayoutToContext("GameUI", "layouts/player.asset", std::shared_ptr(this->_rootContext));
    this->_uiManager->addLayoutToContext("GameUI", "layouts/simulation.asset", std::shared_ptr(this->_rootContext));
}

void GameUISystem::stop() {
    //
}

void GameUISystem::update(float) {
    //
}
