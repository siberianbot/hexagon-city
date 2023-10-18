#ifndef HEXAGON_CITY_GAME_UI_HPP
#define HEXAGON_CITY_GAME_UI_HPP

#include <memory>
#include <optional>

#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Debug/UI/UIContext.hpp>
#include <Penrose/Builtin/Debug/UI/Widgets.hpp>

#include "src/CitySimulationSystem.hpp"
#include "src/PlayerStateContext.hpp"

using namespace Penrose;

class GameUISystem : public Resource, public Initializable, public System {
public:
    explicit GameUISystem(ResourceSet *resources);
    ~GameUISystem() override = default;

    void init() override;
    void destroy() override;

    void update(float) override;

    [[nodiscard]] std::string getName() const override { return "GameUI"; }

private:
    Lazy<ECSManager> _ecsManager;
    Lazy<EventQueue> _eventQueue;
    Lazy<UIContext> _uiContext;
    Lazy<CitySimulationSystem> _citySimulationSystem;
    Lazy<PlayerStateContext> _playerStateContext;

    std::shared_ptr<Container> _cellContainer;
    std::shared_ptr<Container> _buildingContainer;
    std::shared_ptr<Window> _selectionWindow;

    std::shared_ptr<Label> _residentialCount;
    std::shared_ptr<Label> _residentialCapacity;
    std::shared_ptr<Label> _residentialGroupsCount;
    std::shared_ptr<ListBox> _residentialGroups;
    std::shared_ptr<Container> _residentialInsightContainer;

    std::shared_ptr<Label> _industrialEmployeesCapacity;
    std::shared_ptr<Label> _industrialProducts;
    std::shared_ptr<Label> _industrialProductCapacity;
    std::shared_ptr<Label> _industrialProductMultiplier;
    std::shared_ptr<Label> _industrialQueryTimer;
    std::shared_ptr<Container> _industrialInsightContainer;

    std::shared_ptr<Label> _commercialHappiness;
    std::shared_ptr<Label> _commercialHappinessCapacity;
    std::shared_ptr<Label> _commercialHappinessMultiplier;
    std::shared_ptr<Label> _commercialQueryTimer;
    std::shared_ptr<Container> _commercialInsightContainer;

    std::shared_ptr<Label> _playerBalanceLabel;
    std::shared_ptr<Window> _playerWindow;

    std::shared_ptr<Label> _populationIncomeTimerLabel;
    std::shared_ptr<Window> _simulationInsightWindow;

    EventQueue::HandlerIdx _eventHandlerIdx = -1;

    std::optional<Entity> _selection;
};

#endif // HEXAGON_CITY_GAME_UI_HPP
