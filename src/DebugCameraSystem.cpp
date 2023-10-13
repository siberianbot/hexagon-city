#include "DebugCameraSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Events/InputEvent.hpp>
#include <Penrose/Math/Constants.hpp>
#include <Penrose/Math/NumericFuncs.hpp>

#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

DebugCameraSystem::DebugCameraSystem(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()),
          _eventQueue(resources->getLazy<EventQueue>()),
          _inputHandler(resources->getLazy<InputHandler>()),
          _sceneManager(resources->getLazy<SceneManager>()),
          _surfaceManager(resources->getLazy<SurfaceManager>()) {
//
}

void DebugCameraSystem::init() {
    auto root = this->_sceneManager->getOrAddRoot("City");

    auto entity = this->_ecsManager->createEntity();

    auto transform = this->_ecsManager->addComponent<TransformComponent>(entity);
    transform->getPos() = glm::vec3(-1, 6, -1);
    transform->getRot() = glm::vec3(0, glm::radians(-30.0f), glm::radians(-45.0f));

    auto perspective = this->_ecsManager->addComponent<PerspectiveCameraComponent>(entity);
    this->_ecsManager->addComponent<ViewComponent>(entity);

    this->_currentCamera = Camera{
            .entity = entity,
            .rotation = {glm::radians(45.0f), glm::radians(-45.0f)},
            .perspective = perspective,
            .transform = transform,
            .state = CameraState::Unfocused
    };

    this->_sceneManager->insertEntityNode(root, entity);

    this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::InputEvent, InputEventArgs>(
            [this](const InputEvent *event) {

                if (!this->_currentCamera.has_value()) {
                    return;
                }

                switch (event->getArgs().type) {
                    case InputEventType::KeyStateUpdated: {
                        auto [key, state] = event->getArgs().keyState;

                        if (key != InputKey::F1 || state != InputState::Pressed) {
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

                        break;
                    }

                    case InputEventType::MouseMoved: {
                        if (this->_currentCamera->state != CameraState::Focused) {
                            return;
                        }

                        auto [dx, dy] = event->getArgs().mousePosDelta;
                        auto [phi, theta] = this->_currentCamera->rotation;

                        this->_currentCamera->rotation = {
                                cycle<float>(phi - dx, 0.0f, 2 * PI_F),
                                std::clamp(theta + dy, -PI_2_F, PI_2_F)
                        };

                        break;
                    }

                    default:
                        break;
                }
            });
}

void DebugCameraSystem::destroy() {
    this->_eventQueue->removeHandler(this->_eventHandlerIdx);

    if (!this->_currentCamera.has_value()) {
        return;
    }

    auto entity = this->_currentCamera->entity;
    this->_ecsManager->destroyEntity(std::forward<decltype(entity)>(entity));

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
