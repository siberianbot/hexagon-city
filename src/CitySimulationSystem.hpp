#ifndef HEXAGON_CITY_CITY_SIMULATION_SYSTEM_HPP
#define HEXAGON_CITY_CITY_SIMULATION_SYSTEM_HPP

#include <list>
#include <map>
#include <memory>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/BuildingCreatedEvent.hpp"
#include "src/BuildingDestroyedEvent.hpp"
#include "src/BuildingUpgradedEvent.hpp"
#include "src/GridBuildingsSystem.hpp"
#include "src/RandomGenerator.hpp"

using namespace Penrose;

class CitySimulationSystem : public Resource<CitySimulationSystem>,
                             public Initializable,
                             public System {
public:
    struct Resident {
        std::uint32_t count;
        std::uint32_t money;
        float happiness;
        float happinessTimer;
        float restTimer;
    };

    struct ResidentialData {
        std::list<Resident> residents;
        std::uint32_t count;
        std::uint32_t capacity;
    };

    struct IndustrialData {
        std::uint32_t employeesCapacity;
        std::uint32_t products;
        std::uint32_t productCapacity;
        std::uint32_t productMultiplier;
        float queryTimer;
    };

    struct CommercialData {
        std::uint32_t happiness;
        std::uint32_t happinessCapacity;
        std::uint32_t happinessMultiplier;
        float queryTimer;
    };

    explicit CitySimulationSystem(ResourceSet *resources);
    ~CitySimulationSystem() override = default;

    void init() override;
    void destroy() override;

    void update(float delta) override;

    [[nodiscard]] std::string getName() const override { return "CitySimulation"; }

    [[nodiscard]] const float &getPopulationIncomeTimer() const { return this->_populationIncomeTimer; }

    [[nodiscard]] const std::map<Entity, ResidentialData> &getResidentials() const { return this->_residentials; }

    [[nodiscard]] const std::map<Entity, IndustrialData> &getIndustrials() const { return this->_industrials; }

    [[nodiscard]] const std::map<Entity, CommercialData> &getCommercials() const { return this->_commercials; }

private:
    ResourceProxy<EventQueue> _eventQueue;
    ResourceProxy<GridBuildingsSystem> _gridBuildingsSystem;
    ResourceProxy<RandomGenerator> _randomGenerator;

    EventQueue::HandlerIdx _eventHandlerIdx = -1;

    std::map<Entity, ResidentialData> _residentials;
    std::map<Entity, IndustrialData> _industrials;
    std::map<Entity, CommercialData> _commercials;

    float _populationIncomeTimer;

    void handleBuildingCreated(const std::shared_ptr<BuildingCreatedEvent> &event);
    void handleBuildingUpgraded(const std::shared_ptr<BuildingUpgradedEvent> &event);
    void handleBuildingDestroyed(const std::shared_ptr<BuildingDestroyedEvent> &event);

    void updatePopulationIncome(float delta);
    void updateResidentials(float delta);
    void updateIndustrials(float delta);
    void updateCommercials(float delta);
};

#endif // HEXAGON_CITY_CITY_SIMULATION_SYSTEM_HPP
