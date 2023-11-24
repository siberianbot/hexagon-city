#include "DebugCameraSystem.hpp"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Math/Constants.hpp>
#include <Penrose/Math/NumericFuncs.hpp>

#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

DebugCameraSystem::DebugCameraSystem(ResourceSet *resources)
        : _entityManager(resources->get<EntityManager>()),
          _inputEventQueue(resources->get<InputEventQueue>()),
          _inputHandler(resources->get<InputHandler>()),
          _sceneManager(resources->get<SceneManager>()),
          _surfaceManager(resources->get<SurfaceManager>()) {
    //
}

void DebugCameraSystem::init() {
    auto root = this->_sceneManager->getOrAddRoot("City");

    auto entity = this->_entityManager->createEntity();

    auto transform = std::make_shared<TransformComponent>();
    transform->getPos() = glm::vec3(0, 6, 0);
    transform->getRot() = glm::vec3(0, 0, glm::radians(-90.0f));

    auto perspective = std::make_shared<PerspectiveCameraComponent>();

    this->_entityManager->addComponent(entity, transform);
    this->_entityManager->addComponent(entity, perspective);
    this->_entityManager->addComponent(entity, std::make_shared<ViewComponent>());

    this->_currentCamera = Camera{
            .entity = entity,
            .rotation = {glm::radians(0.0f), glm::radians(-90.0f)},
            .perspective = perspective,
            .transform = transform,
            .state = CameraState::Unfocused
    };

    this->_sceneManager->insertEntityNode(root, entity);

    this->_inputEventQueue->addHandler<KeyStateUpdatedEvent>([this](const KeyStateUpdatedEvent *event) {
        if (event->getKey() != InputKey::F1 || event->getState() != InputState::Pressed) {
            return;
        }

        switch (this->_currentCamera->state) {
            case CameraState::Unfocused:
                this->_currentCamera->state = CameraState::Focused;
                this->_surfaceManager->getSurface()->lockCursor();
                break;

            case CameraState::Focused:
                this->_currentCamera->state = CameraState::Unfocused;
                this->_surfaceManager->getSurface()->unlockCursor();
                break;

            default:
                break;
        }
    });

    this->_inputEventQueue->addHandler<MouseMovementEvent>([this](const MouseMovementEvent *event) {
        if (this->_currentCamera->state != CameraState::Focused) {
            return;
        }

        auto [dx, dy] = event->getDelta();
        auto [phi, theta] = this->_currentCamera->rotation;

        this->_currentCamera->rotation = {
                cycle<float>(phi - dx, 0.0f, 2 * PI_F),
                std::clamp(theta + dy, -PI_2_F, PI_2_F)
        };
    });
}

void DebugCameraSystem::destroy() {
    if (!this->_currentCamera.has_value()) {
        return;
    }

    auto entity = this->_currentCamera->entity;
    this->_entityManager->destroyEntity(entity);

    this->_currentCamera = std::nullopt;
}

void DebugCameraSystem::update(float delta) {

    if (!this->_currentCamera.has_value() || this->_currentCamera->state != CameraState::Focused) {
        return;
    }

    auto [phi, theta] = this->_currentCamera->rotation;

    this->_currentCamera->transform->getRot().y = phi;
    this->_currentCamera->transform->getRot().z = theta;

    auto rotation = glm::rotate(glm::mat4(1), this->_currentCamera->transform->getRot().y, glm::vec3(0, 1, 0)) *
                    glm::rotate(glm::mat4(1), this->_currentCamera->transform->getRot().x, glm::vec3(1, 0, 0)) *
                    glm::rotate(glm::mat4(1), this->_currentCamera->transform->getRot().z, glm::vec3(0, 0, 1));

    auto forward = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
    auto up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));
    auto side = glm::vec3(rotation * glm::vec4(0, 0, 1, 1));

    auto shifted = this->_inputHandler->getCurrentStateOf(InputKey::LeftShift) == InputState::Pressed;
    float acc = delta * (shifted ? 2.0f : 1.0f);

    if (this->_inputHandler->getCurrentStateOf(InputKey::W) == InputState::Pressed) {
        this->_currentCamera->transform->getPos() += forward * acc;
    }

    if (this->_inputHandler->getCurrentStateOf(InputKey::S) == InputState::Pressed) {
        this->_currentCamera->transform->getPos() -= forward * acc;
    }

    if (this->_inputHandler->getCurrentStateOf(InputKey::A) == InputState::Pressed) {
        this->_currentCamera->transform->getPos() -= side * acc;
    }

    if (this->_inputHandler->getCurrentStateOf(InputKey::D) == InputState::Pressed) {
        this->_currentCamera->transform->getPos() += side * acc;
    }

    if (this->_inputHandler->getCurrentStateOf(InputKey::Space) == InputState::Pressed) {
        this->_currentCamera->transform->getPos() += up * acc;
    }

    if (this->_inputHandler->getCurrentStateOf(InputKey::LeftControl) == InputState::Pressed) {
        this->_currentCamera->transform->getPos() -= up * acc;
    }
}
