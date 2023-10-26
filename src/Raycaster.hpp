#ifndef HEXAGON_CITY_RAYCASTER_HPP
#define HEXAGON_CITY_RAYCASTER_HPP

#include <optional>

#include <glm/vec3.hpp>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

using namespace Penrose;

class Raycaster : public Resource<Raycaster, ResourceGroup::Custom> {
public:
    explicit Raycaster(ResourceSet *resources);
    ~Raycaster() override = default;

    [[nodiscard]] std::optional<Entity> collide(const glm::vec3 &position,
                                                const glm::vec3 &direction);

private:
    ResourceProxy<EntityManager> _entityManager;
};

#endif // HEXAGON_CITY_RAYCASTER_HPP
