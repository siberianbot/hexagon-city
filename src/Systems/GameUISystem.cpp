#include "GameUISystem.hpp"

#include <fmt/core.h>

#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridCellComponent.hpp"

GameUISystem::GameUISystem(ResourceSet *resources)
        : _entityManager(resources->get<EntityManager>()),
          _inGameEventQueue(resources->get<InGameEventQueue>()),
          _uiContext(resources->get<UIContext>()),
          _citySimulationSystem(resources->get<CitySimulationSystem>()),
          _playerStateContext(resources->get<PlayerStateContext>()) {
    //
}

void GameUISystem::init() {

    auto buildingTypeDropDown = std::shared_ptr<DropDown>(new DropDown( // NOLINT(modernize-make-shared)
            "Building Type",
            {
                    {static_cast<std::uint64_t>(BuildingType::Residential), "Residential"},
                    {static_cast<std::uint64_t>(BuildingType::Commercial),  "Commercial"},
                    {static_cast<std::uint64_t>(BuildingType::Industrial),  "Industrial"}
            }));

    this->_cellContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    buildingTypeDropDown,
                    std::make_shared<Button>("Create building", [this, buildingTypeDropDown]() {
                        if (!buildingTypeDropDown->getSelected().has_value()) {
                            // TODO notify "Choose building type"

                            return;
                        }

                        this->_inGameEventQueue->push<CreateBuildingRequestEvent>(
                                *this->_selection,
                                static_cast<BuildingType>(*buildingTypeDropDown->getSelected())
                        );
                    })
            }));

    this->_buildingContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    std::make_shared<Button>("Upgrade building", [this]() {
                        this->_inGameEventQueue->push<UpgradeBuildingRequestEvent>(*this->_selection);
                    }),
                    std::make_shared<Button>("Destroy building", [this]() {
                        this->_inGameEventQueue->push<DestroyBuildingRequestEvent>(*this->_selection);
                        this->_selection = std::nullopt;
                    })
            }));

    this->_residentialCount = std::make_shared<Label>("Count: 0");
    this->_residentialCapacity = std::make_shared<Label>("Capacity: 0");
    this->_residentialGroupsCount = std::make_shared<Label>("Count of groups: 0");
    this->_residentialGroups = std::make_shared<ListBox>("Groups");
    this->_residentialInsightContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    std::make_shared<Label>("Residential Building"),
                    this->_residentialCount,
                    this->_residentialCapacity,
                    this->_residentialGroupsCount,
                    this->_residentialGroups
            }));

    this->_industrialEmployeesCapacity = std::make_shared<Label>("Employees Capacity: 0");
    this->_industrialProducts = std::make_shared<Label>("Products: 0");
    this->_industrialProductCapacity = std::make_shared<Label>("Product Capacity: 0");
    this->_industrialProductMultiplier = std::make_shared<Label>("Product Multiplier: 0");
    this->_industrialQueryTimer = std::make_shared<Label>("Query Timer: 0");
    this->_industrialInsightContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    std::make_shared<Label>("Industrial Building"),
                    this->_industrialEmployeesCapacity,
                    this->_industrialProducts,
                    this->_industrialProductCapacity,
                    this->_industrialProductMultiplier,
                    this->_industrialQueryTimer,
            }));

    this->_commercialHappiness = std::make_shared<Label>("Happiness: 0");
    this->_commercialHappinessCapacity = std::make_shared<Label>("Happiness Capacity: 0");
    this->_commercialHappinessMultiplier = std::make_shared<Label>("Happiness Multiplier: 0");
    this->_commercialQueryTimer = std::make_shared<Label>("Query Timer: 0");
    this->_commercialInsightContainer = std::shared_ptr<Container>(new Container( // NOLINT(modernize-make-shared)
            {
                    std::make_shared<Label>("Commercial Building"),
                    this->_commercialHappiness,
                    this->_commercialHappinessCapacity,
                    this->_commercialHappinessMultiplier,
                    this->_commercialQueryTimer,
            }));

    this->_selectionWindow = std::shared_ptr<Window>(new Window( // NOLINT(modernize-make-shared)
            "Selection",
            {
                    this->_cellContainer,
                    this->_buildingContainer,
                    this->_residentialInsightContainer,
                    this->_industrialInsightContainer,
                    this->_commercialInsightContainer,
            }
    ));
    this->_selectionWindow->setVisible(false);

    this->_playerBalanceLabel = std::make_shared<Label>("Balance: 0");
    this->_playerWindow = std::shared_ptr<Window>(new Window( // NOLINT(modernize-make-shared)
            "Player",
            {
                    this->_playerBalanceLabel
            }
    ));

    this->_populationIncomeTimerLabel = std::make_shared<Label>("Population income timer: 0");
    this->_simulationInsightWindow = std::shared_ptr<Window>(new Window( // NOLINT(modernize-make-shared)
            "Simulation Insight",
            {
                    this->_populationIncomeTimerLabel
            }
    ));

    this->_uiContext->setRoot("Selection", this->_selectionWindow);
    this->_uiContext->setRoot("Player", this->_playerWindow);
    this->_uiContext->setRoot("SimulationInsight", this->_simulationInsightWindow);

    this->_inGameEventQueue->addHandler<SelectionChangedEvent>([this](const SelectionChangedEvent *event) {
        this->_selection = event->getSelection();
    });
}

void GameUISystem::destroy() {
    this->_uiContext->removeRoot("Selection");
    this->_uiContext->removeRoot("Player");
    this->_uiContext->removeRoot("SimulationInsight");

    this->_selectionWindow = nullptr;
    this->_playerWindow = nullptr;
    this->_simulationInsightWindow = nullptr;
}

void GameUISystem::update(float) {

    this->_playerBalanceLabel->setText(fmt::format("Balance: {}", this->_playerStateContext->balance()));
    this->_populationIncomeTimerLabel->setText(
            fmt::format("Population income timer: {}", this->_citySimulationSystem->getPopulationIncomeTimer())
    );

    this->_selectionWindow->setVisible(this->_selection.has_value());

    if (this->_selection.has_value()) {
        this->_cellContainer->setVisible(this->_entityManager->hasComponent<GridCellComponent>(*this->_selection));

        auto maybeBuilding = this->_entityManager->tryGetComponent<GridBuildingComponent>(*this->_selection);

        this->_buildingContainer->setVisible(maybeBuilding.has_value());
        this->_residentialInsightContainer->setVisible(maybeBuilding.has_value() &&
                                                       (*maybeBuilding)->buildingType() == BuildingType::Residential);
        this->_industrialInsightContainer->setVisible(maybeBuilding.has_value() &&
                                                      (*maybeBuilding)->buildingType() == BuildingType::Industrial);
        this->_commercialInsightContainer->setVisible(maybeBuilding.has_value() &&
                                                      (*maybeBuilding)->buildingType() == BuildingType::Commercial);

        if (maybeBuilding.has_value()) {
            switch ((*maybeBuilding)->buildingType()) {
                case BuildingType::Residential: {
                    auto maybeResidential = tryGetPointer(this->_citySimulationSystem->getResidentials(),
                                                          *this->_selection);

                    if (maybeResidential.has_value()) {
                        this->_residentialCount->setText(
                                fmt::format("Count: {}", (*maybeResidential)->count));

                        this->_residentialCapacity->setText(
                                fmt::format("Capacity: {}", (*maybeResidential)->capacity));

                        this->_residentialGroupsCount->setText(
                                fmt::format("Count of groups: {}", (*maybeResidential)->residents.size()));

                        std::uint32_t idx = 0;
                        std::vector<ListBox::Item> items;

                        for (const auto &resident: (*maybeResidential)->residents) {
                            auto item = fmt::format(
                                    "[{}] count = {}\n"
                                    "     happiness = {}\n"
                                    "     money = {}\n"
                                    "     happiness timer = {}\n"
                                    "     rest timer = {} \n",
                                    idx, resident.count, resident.happiness, resident.money,
                                    resident.happinessTimer, resident.restTimer);

                            items.emplace_back(idx++, item);
                        }

                        this->_residentialGroups->setItems(std::forward<decltype(items)>(items));
                    }

                    break;
                }

                case BuildingType::Industrial: {
                    auto maybeIndustrial = tryGetPointer(this->_citySimulationSystem->getIndustrials(),
                                                         *this->_selection);

                    if (maybeIndustrial.has_value()) {
                        this->_industrialEmployeesCapacity->setText(
                                fmt::format("Employees Capacity: {}", (*maybeIndustrial)->employeesCapacity));

                        this->_industrialProducts->setText(
                                fmt::format("Products: {}", (*maybeIndustrial)->products));

                        this->_industrialProductCapacity->setText(
                                fmt::format("Product Capacity: {}", (*maybeIndustrial)->productCapacity));

                        this->_industrialProductMultiplier->setText(
                                fmt::format("Product Multiplier: {}", (*maybeIndustrial)->productMultiplier));

                        this->_industrialQueryTimer->setText(
                                fmt::format("Query Timer: {}", (*maybeIndustrial)->queryTimer));
                    }

                    break;
                }

                case BuildingType::Commercial: {
                    auto maybeCommercial = tryGetPointer(this->_citySimulationSystem->getCommercials(),
                                                         *this->_selection);

                    if (maybeCommercial.has_value()) {
                        this->_commercialHappiness->setText(
                                fmt::format("Happiness: {}", (*maybeCommercial)->happiness));

                        this->_commercialHappinessCapacity->setText(
                                fmt::format("Happiness Capacity: {}", (*maybeCommercial)->happinessCapacity));

                        this->_commercialHappinessMultiplier->setText(
                                fmt::format("Happiness Multiplier: {}", (*maybeCommercial)->happinessMultiplier));

                        this->_commercialQueryTimer->setText(
                                fmt::format("Query Timer: {}", (*maybeCommercial)->queryTimer));
                    }

                    break;
                }

                default:
                    throw std::logic_error("Building type is not supported");
            }
        }
    }
}
