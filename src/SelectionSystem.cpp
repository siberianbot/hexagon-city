#include "SelectionSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Events/InputEvent.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>

#include "src/GridCellComponent.hpp"
#include "src/GridConstants.hpp"

SelectionSystem::SelectionSystem(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()),
          _eventQueue(resources->getLazy<EventQueue>()),
          _inputHandler(resources->getLazy<InputHandler>()),
          _raycaster(resources->getLazy<Raycaster>()),
          _surfaceManager(resources->getLazy<SurfaceManager>()),
          _cameraSystem(resources->getLazy<DebugCameraSystem>()) {
    //
}

void SelectionSystem::init() {
    this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::InputEvent, InputEventArgs>(
            [this](const InputEvent *event) {
                // TODO
            });
}

void SelectionSystem::destroy() {
    this->_eventQueue->removeHandler(this->_eventHandlerIdx);
}

void SelectionSystem::update(float) {
    const auto &currentCamera = this->_cameraSystem->getCurrentCamera();

    auto [w, h] = this->_surfaceManager->getSurface()->getSize();
    auto [x, y] = this->_inputHandler->getCurrentMousePosition();

    auto n = glm::vec4(x, y, 2 * currentCamera.perspective->getNear() - 1, 1);

    auto projection = glm::perspective(
            currentCamera.perspective->getFov(),
            static_cast<float>(w) / static_cast<float>(h),
            currentCamera.perspective->getNear(),
            currentCamera.perspective->getFar()
    );

    auto projectionInverse = glm::inverse(projection);

    auto v = projectionInverse * n;

    v = v / v.w;

    auto rotation = glm::rotate(glm::mat4(1), currentCamera.transform->getRot().y,
                                glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1), currentCamera.transform->getRot().x,
                                glm::vec3(1, 0, 0)) *
                    glm::rotate(glm::mat4(1), currentCamera.transform->getRot().z,
                                glm::vec3(0, 0, 1));

    auto forward = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
    auto up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));

    auto view = glm::lookAt(currentCamera.transform->getPos(),
                            currentCamera.transform->getPos() + forward, up);

    auto viewInverse = glm::inverse(view);

    auto world = viewInverse * v;

    auto position = glm::vec4(currentCamera.transform->getPos(), 1);
    auto direction = world - position;

    auto newSelection = this->_raycaster->collide(position, direction);

    if (this->_selection.has_value()) {
        auto gridCell = this->_ecsManager->getComponent<GridCellComponent>(*this->_selection);
        auto meshRenderer = this->_ecsManager->getComponent<MeshRendererComponent>(*this->_selection);

        meshRenderer->setColor(GRID_CELL_COLORS.at(static_cast<std::uint32_t>(gridCell->getType())));
    }

    if (newSelection.has_value()) {
        this->_ecsManager->getComponent<MeshRendererComponent>(*newSelection)->setColor(GRID_CELL_SELECTED_COLOR);
    }

    this->_selection = newSelection;
}
