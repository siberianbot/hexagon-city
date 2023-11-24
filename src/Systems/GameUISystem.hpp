#ifndef HEXAGON_CITY_SYSTEMS_GAME_UI_SYSTEM_HPP
#define HEXAGON_CITY_SYSTEMS_GAME_UI_SYSTEM_HPP

#include <memory>
#include <optional>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Resources/Runnable.hpp>
#include <Penrose/UI/UIManager.hpp>

#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridCellComponent.hpp"
#include "src/InGameEvents.hpp"
#include "src/PlayerStateContext.hpp"
#include "src/Systems/CitySimulationSystem.hpp"

using namespace Penrose;

class GameUISystem: public Resource<GameUISystem, ResourceGroup::ECSSystem>,
                    public Initializable,
                    public Runnable,
                    public System {
public:
    explicit GameUISystem(ResourceSet *resources);
    ~GameUISystem() override = default;

    void init() override;
    void destroy() override;

    void run() override;
    void stop() override;

    void update(float) override;

    [[nodiscard]] std::string getName() const override { return "GameUI"; }

private:
    struct SelectionData {
        Entity entity;
        std::optional<std::shared_ptr<GridCellComponent>> cell;
        std::optional<std::shared_ptr<GridBuildingComponent>> building;
        std::optional<const CitySimulationSystem::ResidentialData *> residential;
        std::optional<const CitySimulationSystem::IndustrialData *> industrial;
        std::optional<const CitySimulationSystem::CommercialData *> commercial;
    };

    ResourceProxy<EntityManager> _entityManager;
    ResourceProxy<InGameEventQueue> _inGameEventQueue;
    ResourceProxy<UIManager> _uiManager;
    ResourceProxy<CitySimulationSystem> _citySimulationSystem;
    ResourceProxy<PlayerStateContext> _playerStateContext;

    std::shared_ptr<ObjectValue> _rootContext;
    std::optional<SelectionData> _selection;
};

#endif // HEXAGON_CITY_SYSTEMS_GAME_UI_SYSTEM_HPP
