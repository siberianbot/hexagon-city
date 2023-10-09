#include "Raycaster.hpp"

#include <map>
#include <ranges>
#include <tuple>

#include <glm/geometric.hpp>

#include "src/GridBuildingComponent.hpp"
#include "src/GridCellComponent.hpp"
#include "src/GridConstants.hpp"
#include "src/GridPositionComponent.hpp"
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
                      std::views::filter([&](const ECSEntry &entry) {
                          return this->_ecsManager->tryGetComponent<GridPositionComponent>(entry.entity).has_value();
                      }) |
                      std::views::transform([&](const ECSEntry &entry) {
                          auto component = this->_ecsManager->getComponent<GridPositionComponent>(entry.entity);
                          auto position = glm::vec3(sqrt(3) * component->column() + sqrt(3) / 2 * component->row(),
                                                    0,
                                                    3. / 2 * component->row());

                          return std::make_tuple(entry.entity, position);
                      }) |
                      std::views::filter([&](const std::tuple<Entity, glm::vec3> &tuple) {
                          const auto &[entity, entityPosition] = tuple;

                          auto scale = glm::vec3(1);

                          auto maybeBuilding = this->_ecsManager->tryGetComponent<GridBuildingComponent>(entity);
                          auto maybeCell = this->_ecsManager->tryGetComponent<GridCellComponent>(entity);

                          if (maybeBuilding.has_value()) {
                              scale.y = (*maybeBuilding)->level() * GRID_BUILDING_PER_LEVEL_HEIGHT;
                          }

                          if (maybeCell.has_value()) {
                              scale.y = GRID_CELL_HEIGHT;
                          }

                          auto boxMin = entityPosition - scale;
                          auto boxMax = entityPosition + scale;

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
                              return false;
                          }

                          auto intersection = position + (direction * maximum);
                          const float eps = 0.0001f;

                          bool matched = true;
                          for (glm::vec3::length_type idx = 0; idx < 3; idx++) {
                              if (intersection[idx] + eps < boxMin[idx] || intersection[idx] - eps > boxMax[idx]) {
                                  matched = false;
                              }
                          }

                          return matched;
                      }) |
                      std::views::transform([&](const std::tuple<Entity, glm::vec3> &tuple) {
                          const auto &[entity, entityPosition] = tuple;

                          return std::make_tuple(entity, glm::length(position - entityPosition));
                      });

    std::map<Entity, float> entityDistances;
    for (const auto &[entity, distance]: candidates) {
        entityDistances.emplace(entity, distance);
    }

    if (entityDistances.empty()) {
        return std::nullopt;
    }

    auto goodEntity = entityDistances.begin()->first;
    for (const auto &[entity, distance]: entityDistances) {
        if (entityDistances[goodEntity] > distance) {
            goodEntity = entity;
        }
    }

    return goodEntity;
}
