#include "Raycaster.hpp"

#include <ranges>
#include <tuple>

#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

#include "src/RayCollisionVolumeComponent.hpp"

template<typename T>
constexpr inline T max(const T &a, const T &b) {
    return a < b ? b : a;
}

template<typename T, typename ...Args>
constexpr inline T max(const T &a, const T &b, const Args &...args) {
    return max(max(a, b), args...);
}

Raycaster::Raycaster(ResourceSet *resources)
        : _ecsManager(resources->getLazy<ECSManager>()) {
    //
}

std::optional<Entity> Raycaster::collide(const glm::vec3 &position, const glm::vec3 &direction) {

    auto query = ECSQuery().component<RayCollisionVolumeComponent>();

    auto candidates = this->_ecsManager->query(query) |
                      std::views::transform([&](const ECSEntry &entry) {
                          auto transform = this->_ecsManager->getComponent<TransformComponent>(entry.entity);

                          return std::make_tuple(entry.entity, transform);
                      });

    for (const auto &[entity, transform]: candidates) {
        auto boxMin = transform->getPos() - transform->getScale();
        auto boxMax = transform->getPos() + transform->getScale();

        auto result = glm::vec3(-1);

        for (glm::vec3::length_type idx = 0; idx < 3; idx++) {
            if (direction[idx] > 0) {
                result[idx] = (boxMin[idx] - position[idx]) / direction[idx];
            } else if (direction[idx] < 0) {
                result[idx] = (boxMax[idx] - position[idx]) / direction[idx];
            }
        }

        auto maximum = max(result.x, result.y, result.z);

        if (maximum < 0) {
            continue;
        }

        auto intersection = position + (direction * maximum);
        const float eps = 0.0001f;

        bool matched = true;
        for (glm::vec3::length_type idx = 0; idx < 3; idx++) {
            if (intersection[idx] + eps < boxMin[idx] || intersection[idx] - eps > boxMax[idx]) {
                matched = false;
            }
        }

        if (matched) {
            return entity;
        }
    }

    return std::nullopt;
}
