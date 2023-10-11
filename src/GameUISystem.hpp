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

    std::shared_ptr<Container> _cellContainer;
    std::shared_ptr<Container> _buildingContainer;
    std::shared_ptr<Window> _selectionWindow;

    EventQueue::HandlerIdx _eventHandlerIdx = -1;

    std::optional<Entity> _selection;
};

#endif // HEXAGON_CITY_GAME_UI_HPP
