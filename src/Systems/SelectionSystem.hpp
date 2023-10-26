#ifndef HEXAGON_CITY_SYSTEMS_SELECTION_SYSTEM_HPP
#define HEXAGON_CITY_SYSTEMS_SELECTION_SYSTEM_HPP

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include "src/InGameEvents.hpp"
#include "src/Raycaster.hpp"
#include "src/Systems/DebugCameraSystem.hpp"

using namespace Penrose;

class SelectionSystem : public Resource<SelectionSystem, ResourceGroup::ECSSystem>,
                        public System {
public:
    explicit SelectionSystem(ResourceSet *resources);
    ~SelectionSystem() override = default;

    void update(float) override;

    [[nodiscard]] std::string getName() const override { return "Selection"; }

private:
    ResourceProxy<EntityManager> _entityManager;
    ResourceProxy<InGameEventQueue> _inGameEventQueue;
    ResourceProxy<InputHandler> _inputHandler;
    ResourceProxy<Raycaster> _raycaster;
    ResourceProxy<SurfaceManager> _surfaceManager;

    ResourceProxy<DebugCameraSystem> _cameraSystem; // TODO: we are not gonna always use debug camera

    std::optional<Entity> _hovered;
    std::optional<Entity> _selected;
};

#endif // HEXAGON_CITY_SYSTEMS_SELECTION_SYSTEM_HPP
