#ifndef HEXAGON_CITY_SELECTION_SYSTEM_HPP
#define HEXAGON_CITY_SELECTION_SYSTEM_HPP

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include "src/DebugCameraSystem.hpp"
#include "src/Raycaster.hpp"

using namespace Penrose;

class SelectionSystem : public Resource, public System {
public:
    explicit SelectionSystem(ResourceSet *resources);
    ~SelectionSystem() override = default;

    void update(float) override;

    [[nodiscard]] std::string getName() const override { return "Selection"; }

private:
    Lazy<ECSManager> _ecsManager;
    Lazy<InputHandler> _inputHandler;
    Lazy<Raycaster> _raycaster;
    Lazy<SurfaceManager> _surfaceManager;

    Lazy<DebugCameraSystem> _cameraSystem; // TODO: we are not gonna always use debug camera

    std::optional<Entity> _hovered;
    std::optional<Entity> _selected;
};

#endif // HEXAGON_CITY_SELECTION_SYSTEM_HPP
