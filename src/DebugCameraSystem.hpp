#ifndef HEXAGON_CITY_DEBUG_CAMERA_SYSTEM_HPP
#define HEXAGON_CITY_DEBUG_CAMERA_SYSTEM_HPP

#include <memory>
#include <optional>
#include <tuple>

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

#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

#include "src/Raycaster.hpp"

using namespace Penrose;

class DebugCameraSystem : public Resource, public Initializable, public System {
public:
    enum class CameraState {
        Unfocused,
        Focused
    };

    struct Camera {
        Entity entity;
        std::tuple<float, float> rotation;
        std::shared_ptr<PerspectiveCameraComponent> perspective;
        std::shared_ptr<TransformComponent> transform;
        CameraState state;
    };

    explicit DebugCameraSystem(ResourceSet *resources);
    ~DebugCameraSystem() override = default;

    void init() override;
    void destroy() override;

    void update(float delta) override;

    [[nodiscard]] std::string getName() const override { return "DebugCamera"; }

    [[nodiscard]] const Camera &getCurrentCamera() const { return (*this->_currentCamera); }

private:
    Lazy<ECSManager> _ecsManager;
    Lazy<EventQueue> _eventQueue;
    Lazy<InputHandler> _inputHandler;
    Lazy<SceneManager> _sceneManager;
    Lazy<SurfaceManager> _surfaceManager;

    std::optional<Camera> _currentCamera;
    EventQueue::HandlerIdx _eventHandlerIdx = -1;
};

#endif // HEXAGON_CITY_DEBUG_CAMERA_SYSTEM_HPP
