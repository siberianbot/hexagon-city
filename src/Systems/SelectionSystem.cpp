#include "SelectionSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "src/GridConstants.hpp"
#include "src/Components/HoveredComponent.hpp"
#include "src/Components/SelectedComponent.hpp"

SelectionSystem::SelectionSystem(ResourceSet *resources)
        : _entityManager(resources->get<EntityManager>()),
          _inGameEventQueue(resources->get<InGameEventQueue>()),
          _inputHandler(resources->get<InputHandler>()),
          _raycaster(resources->get<Raycaster>()),
          _surfaceManager(resources->get<SurfaceManager>()),
          _cameraSystem(resources->get<DebugCameraSystem>()) {
    //
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

    auto newHovered = this->_raycaster->collide(position, direction);

    if (newHovered != this->_hovered) {

        if (newHovered.has_value()) {
            this->_entityManager->addComponent(*newHovered, std::make_shared<HoveredComponent>());
        }

        if (this->_hovered.has_value()) {
            this->_entityManager->removeComponent<HoveredComponent>(*this->_hovered);
        }

        this->_hovered = newHovered;
    }

    if (this->_inputHandler->getCurrentStateOf(InputKey::MB0) != Penrose::InputState::Pressed) {
        return;
    }

    if (this->_hovered != this->_selected) {

        if (this->_selected.has_value()) {
            this->_entityManager->removeComponent<SelectedComponent>(*this->_selected);
        }

        if (this->_hovered.has_value()) {
            this->_entityManager->addComponent(*this->_hovered, std::make_shared<SelectedComponent>());
        }

        this->_selected = this->_hovered;

        this->_inGameEventQueue->push<SelectionChangedEvent>(this->_selected);
    }
}
