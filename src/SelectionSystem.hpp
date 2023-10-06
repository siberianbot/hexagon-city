#ifndef HEXAGON_CITY_SELECTION_SYSTEM_HPP
#define HEXAGON_CITY_SELECTION_SYSTEM_HPP

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include "src/DebugCameraSystem.hpp"
#include "src/Raycaster.hpp"

using namespace Penrose;

class SelectionSystem : public Resource, public Initializable, public System {
public:
    explicit SelectionSystem(ResourceSet *resources);
    ~SelectionSystem() override = default;

    void init() override;
    void destroy() override;

    void update(float) override;

    [[nodiscard]] std::string getName() const override { return "Selection"; }

private:
    Lazy<ECSManager> _ecsManager;
    Lazy<EventQueue> _eventQueue;
    Lazy<InputHandler> _inputHandler;
    Lazy<Raycaster> _raycaster;
    Lazy<SurfaceManager> _surfaceManager;

    Lazy<DebugCameraSystem> _cameraSystem;

    EventQueue::HandlerIdx _eventHandlerIdx = -1;

    std::optional<Entity> _selection;
};

#endif // HEXAGON_CITY_SELECTION_SYSTEM_HPP
