#include "Raycaster.hpp"

#include <map>
#include <ranges>
#include <tuple>

#include <glm/geometric.hpp>

#include <Penrose/Math/NumericFuncs.hpp>

#include "src/GridConstants.hpp"
#include "src/Components/GridBuildingComponent.hpp"
#include "src/Components/GridCellComponent.hpp"
#include "src/Components/GridPositionComponent.hpp"
#include "src/Components/RayCollisionVolumeComponent.hpp"

Raycaster::Raycaster(ResourceSet *resources)
        : _entityManager(resources->get<EntityManager>()) {
    //
}

std::optional<Entity> Raycaster::collide(const glm::vec3 &position, const glm::vec3 &direction) {

    auto candidates = this->_entityManager->query()
                              .component<RayCollisionVolumeComponent>()
                              .all() |
                      std::views::filter([&](const EntityManager::Entry &entry) {
                          return this->_entityManager->hasComponent<GridPositionComponent>(entry.entity);
                      }) |
                      std::views::transform([&](const EntityManager::Entry &entry) {
                          auto component = this->_entityManager->getComponent<GridPositionComponent>(entry.entity);
                          auto position = glm::vec3(sqrt(3) * component->column() + sqrt(3) / 2 * component->row(),
                                                    0,
                                                    3. / 2 * component->row());

                          return std::make_tuple(entry.entity, position);
                      }) |
                      std::views::filter([&](const std::tuple<Entity, glm::vec3> &tuple) {
                          const auto &[entity, entityPosition] = tuple;

                          auto scale = glm::vec3(1);

                          auto maybeBuilding = this->_entityManager->tryGetComponent<GridBuildingComponent>(entity);
                          auto maybeCell = this->_entityManager->tryGetComponent<GridCellComponent>(entity);

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
