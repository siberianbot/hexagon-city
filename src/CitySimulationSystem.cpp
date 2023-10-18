#include "CitySimulationSystem.hpp"

#include <cstdint>
#include <stdexcept>

constexpr static const std::uint32_t RESIDENTIAL_BASE_CAPACITY = 500;

constexpr static const std::uint32_t INDUSTRIAL_BASE_EMPLOYEES_CAPACITY = 200;
constexpr static const std::uint32_t INDUSTRIAL_BASE_PRODUCT_CAPACITY = 500;
constexpr static const std::uint32_t INDUSTRIAL_BASE_PRODUCT_MULTIPLIER = 1;
constexpr static const float INDUSTRIAL_QUERY_TIMER = 5.0f;

constexpr static const std::uint32_t COMMERCIAL_BASE_HAPPINESS_CAPACITY = 500;
constexpr static const std::uint32_t COMMERCIAL_BASE_HAPPINESS_MULTIPLIER = 1;
constexpr static const float COMMERCIAL_QUERY_TIMER = 5.0f;

constexpr static const float POPULATION_TIMER = 15.0f;

constexpr static const std::uint32_t POPULATION_MIN_COUNT = 5;
constexpr static const std::uint32_t POPULATION_MAX_COUNT = 50;
constexpr static const std::uint32_t POPULATION_MIN_MONEY = 200;
constexpr static const std::uint32_t POPULATION_MAX_MONEY = 1000;
constexpr static const float POPULATION_HAPPINESS_TIMER = 5.0f;
constexpr static const float POPULATION_REST_TIMER = 5.0f;

CitySimulationSystem::CitySimulationSystem(ResourceSet *resources)
        : _eventQueue(resources->getLazy<EventQueue>()),
          _gridBuildingsSystem(resources->getLazy<GridBuildingsSystem>()),
          _randomGenerator(resources->getLazy<RandomGenerator>()) {
    //
}

void CitySimulationSystem::init() {
    this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::CustomEvent, CustomEventArgs>(
            [this](const CustomEvent *event) {
                if (event->getArgs().type == BuildingCreatedEvent::name()) {
                    this->handleBuildingCreated(
                            std::dynamic_pointer_cast<BuildingCreatedEvent>(event->getArgs().data)
                    );
                }

                if (event->getArgs().type == BuildingUpgradedEvent::name()) {
                    this->handleBuildingUpgraded(
                            std::dynamic_pointer_cast<BuildingUpgradedEvent>(event->getArgs().data)
                    );
                }

                if (event->getArgs().type == BuildingDestroyedEvent::name()) {
                    this->handleBuildingDestroyed(
                            std::dynamic_pointer_cast<BuildingDestroyedEvent>(event->getArgs().data)
                    );
                }
            });

    this->_residentials.clear();
    this->_industrials.clear();
    this->_commercials.clear();
    this->_populationIncomeTimer = POPULATION_TIMER;
}

void CitySimulationSystem::destroy() {
    this->_eventQueue->removeHandler(this->_eventHandlerIdx);
}

void CitySimulationSystem::update(float delta) {
    this->updatePopulationIncome(delta);
    this->updateResidentials(delta);
    this->updateIndustrials(delta);
    this->updateCommercials(delta);
}

void CitySimulationSystem::handleBuildingCreated(const std::shared_ptr<BuildingCreatedEvent> &event) {
    switch (event->getBuildingType()) {
        case GridBuildingType::Residential:
            this->_residentials.emplace(
                    event->getBuildingEntity(),
                    ResidentialData{
                            .residents = {},
                            .count = 0,
                            .capacity = RESIDENTIAL_BASE_CAPACITY
                    }
            );
            break;

        case GridBuildingType::Industrial:
            this->_industrials.emplace(
                    event->getBuildingEntity(),
                    IndustrialData{
                            .employeesCapacity = INDUSTRIAL_BASE_EMPLOYEES_CAPACITY,
                            .products = 0,
                            .productCapacity = INDUSTRIAL_BASE_PRODUCT_CAPACITY,
                            .productMultiplier = INDUSTRIAL_BASE_PRODUCT_MULTIPLIER,
                            .queryTimer = INDUSTRIAL_QUERY_TIMER
                    }
            );
            break;

        case GridBuildingType::Commercial:
            this->_commercials.emplace(
                    event->getBuildingEntity(),
                    CommercialData{
                            .happiness = 0,
                            .happinessCapacity = COMMERCIAL_BASE_HAPPINESS_CAPACITY,
                            .happinessMultiplier = COMMERCIAL_BASE_HAPPINESS_MULTIPLIER,
                            .queryTimer = COMMERCIAL_QUERY_TIMER
                    }
            );
            break;

        default:
            throw std::logic_error("Unsupported building type");
    }
}

void CitySimulationSystem::handleBuildingUpgraded(const std::shared_ptr<BuildingUpgradedEvent> &event) {
    switch (event->getBuildingType()) {
        case GridBuildingType::Residential: {
            auto residential = orElseThrow(
                    tryGetPointer(this->_residentials, event->getBuildingEntity()),
                    std::logic_error("No such residential building data")
            );

            residential->capacity = event->getNewLevel() * RESIDENTIAL_BASE_CAPACITY;

            break;
        }

        case GridBuildingType::Industrial: {
            auto industrial = orElseThrow(
                    tryGetPointer(this->_industrials, event->getBuildingEntity()),
                    std::logic_error("No such industrial building data")
            );

            industrial->employeesCapacity = event->getNewLevel() * INDUSTRIAL_BASE_EMPLOYEES_CAPACITY;
            industrial->productCapacity = event->getNewLevel() * INDUSTRIAL_BASE_PRODUCT_CAPACITY;
            industrial->productMultiplier = 2 * industrial->productMultiplier;

            break;
        }

        case GridBuildingType::Commercial: {
            auto commercial = orElseThrow(
                    tryGetPointer(this->_commercials, event->getBuildingEntity()),
                    std::logic_error("No such commercial building data")
            );

            commercial->happinessCapacity = event->getNewLevel() * COMMERCIAL_BASE_HAPPINESS_CAPACITY;
            commercial->happinessMultiplier = 2 * commercial->happinessMultiplier;

            break;
        }

        default:
            throw std::logic_error("Unsupported building type");
    }
}

void CitySimulationSystem::handleBuildingDestroyed(const std::shared_ptr<BuildingDestroyedEvent> &event) {
    switch (event->getBuildingType()) {
        case GridBuildingType::Residential:
            this->_residentials.erase(event->getBuildingEntity());
            break;

        case GridBuildingType::Industrial:
            this->_industrials.erase(event->getBuildingEntity());
            break;

        case GridBuildingType::Commercial:
            this->_commercials.erase(event->getBuildingEntity());
            break;

        default:
            throw std::logic_error("Unsupported building type");
    }
}

void CitySimulationSystem::updatePopulationIncome(float delta) {
    this->_populationIncomeTimer -= delta;

    if (this->_populationIncomeTimer > 0) {
        return;
    }

    auto count = this->_randomGenerator->generateInt(POPULATION_MIN_COUNT, POPULATION_MAX_COUNT);

    for (auto &[entity, data]: this->_residentials) {
        if (data.count + count >= data.capacity) {
            continue;
        }

        data.count += count;
        data.residents.emplace_back(
                Resident{
                        .count = count,
                        .money = this->_randomGenerator->generateInt(POPULATION_MIN_MONEY, POPULATION_MAX_MONEY),
                        .happiness = 0.5f,
                        .happinessTimer = POPULATION_HAPPINESS_TIMER,
                        .restTimer = POPULATION_REST_TIMER
                }
        );
    }

    this->_populationIncomeTimer = POPULATION_TIMER;
}

void CitySimulationSystem::updateResidentials(float delta) {
    for (auto &[residential, residentialData]: this->_residentials) {

        std::vector<std::list<Resident>::iterator> toRemoval;

        for (auto it = residentialData.residents.begin(); it != residentialData.residents.end(); it++) {
            auto &resident = *it;

            if (resident.happinessTimer > 0) {
                resident.happinessTimer -= delta;
                resident.happiness += (1 - resident.happiness) *
                                      delta *
                                      this->_randomGenerator->generateFloat(0.01f, 0.25f);
            } else if (resident.happiness > 0) {
                resident.happiness -= (1 - resident.happiness) *
                                      delta *
                                      this->_randomGenerator->generateFloat(0.01f, 0.25f);
            } else {
                toRemoval.push_back(it);

                continue;
            }

            if (resident.restTimer > 0) {
                resident.restTimer -= delta;
            }
        }

        for (const auto &it: toRemoval) {
            residentialData.count -= it->count;
            residentialData.residents.erase(it);
        }
    }
}

void CitySimulationSystem::updateIndustrials(float delta) {
    for (auto &[industrial, industrialData]: this->_industrials) {
        industrialData.queryTimer -= delta;

        if (industrialData.queryTimer > 0) {
            continue;
        }

        std::uint32_t totalCount = 0;
        auto nearestResidentials = this->_gridBuildingsSystem->getAllNearest(industrial, GridBuildingType::Residential);

        for (const auto &residential: nearestResidentials) {
            if (industrialData.products == industrialData.productCapacity) {
                break;
            }

            auto residentialData = orElseThrow(
                    tryGetPointer(this->_residentials, residential),
                    std::logic_error("No such residential building data")
            );

            if (totalCount + residentialData->count > industrialData.employeesCapacity) {
                break;
            }

            totalCount += residentialData->count;

            auto residentialProduct = industrialData.productMultiplier *
                                      this->_randomGenerator->generateInt(10, 100);

            auto residentialIncome = residentialProduct * residentialData->count;

            for (auto &resident: residentialData->residents) {
                if (resident.restTimer > 0) {
                    continue;
                }

                auto residentMultiplier = resident.happiness *
                                          (static_cast<float>(resident.count) /
                                           static_cast<float>(residentialData->count));

                resident.restTimer = POPULATION_REST_TIMER;
                resident.money += static_cast<std::uint32_t>(
                        resident.happiness *
                        (static_cast<float>(resident.count) / static_cast<float>(residentialData->count)) *
                        static_cast<float>(residentialIncome)
                );

                industrialData.products += std::min(
                        industrialData.productCapacity - industrialData.products,
                        static_cast<std::uint32_t>(residentMultiplier * static_cast<float>(residentialProduct))
                );
            }
        }

        industrialData.queryTimer = INDUSTRIAL_QUERY_TIMER;
    }
}

void CitySimulationSystem::updateCommercials(float delta) {
    for (auto &[commercial, commercialData]: this->_commercials) {
        commercialData.queryTimer -= delta;

        if (commercialData.queryTimer > 0) {
            continue;
        }

        auto nearestIndustrials = this->_gridBuildingsSystem->getAllNearest(commercial,
                                                                            GridBuildingType::Industrial);

        for (const auto &industrial: nearestIndustrials) {
            if (commercialData.happiness == commercialData.happinessCapacity) {
                break;
            }

            auto industrialData = orElseThrow(
                    tryGetPointer(this->_industrials, industrial),
                    std::logic_error("No such industrial building data")
            );

            auto consumptionMultiplier = commercialData.happinessMultiplier *
                                         this->_randomGenerator->generateInt(10, 100);

            industrialData->products -= std::min(industrialData->products, consumptionMultiplier);
            commercialData.happiness += std::min(
                    commercialData.happinessCapacity - commercialData.happiness,
                    consumptionMultiplier
            );
        }

        auto nearestResidentials = this->_gridBuildingsSystem->getAllNearest(commercial,
                                                                             GridBuildingType::Residential);

        for (const auto &residential: nearestResidentials) {
            auto residentialData = orElseThrow(
                    tryGetPointer(this->_residentials, residential),
                    std::logic_error("No such residential building data")
            );

            if (commercialData.happiness == 0) {
                break;
            }

            for (auto &resident: residentialData->residents) {
                if (commercialData.happiness == 0) {
                    break;
                }

                if (resident.happinessTimer > 0) {
                    continue;
                }

                auto multiplier = (static_cast<float>(commercialData.happinessMultiplier) / 4) *
                                  (static_cast<float>(resident.count) / static_cast<float>(residentialData->count)) *
                                  this->_randomGenerator->generateFloat(0.25, 1);

                auto money = static_cast<std::uint32_t>(
                        multiplier *
                        (static_cast<float>(6 - nearestResidentials.size()) / 6) *
                        static_cast<float>(this->_randomGenerator->generateInt(100, 500))
                );

                if (resident.money < money) {
                    continue;
                }

                auto happiness = static_cast<std::uint32_t>(
                        multiplier *
                        static_cast<float>(this->_randomGenerator->generateInt(100, 500))
                );

                resident.money -= money;
                commercialData.happiness -= std::min(commercialData.happiness, happiness);

                resident.happinessTimer = POPULATION_HAPPINESS_TIMER;
            }
        }

        commercialData.queryTimer = COMMERCIAL_QUERY_TIMER;
    }
}
