#ifndef HEXAGON_CITY_COMPONENTS_RAY_COLLISION_VOLUME_COMPONENT_HPP
#define HEXAGON_CITY_COMPONENTS_RAY_COLLISION_VOLUME_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

using namespace Penrose;

class RayCollisionVolumeComponent : public Component<RayCollisionVolumeComponent> {
public:
    ~RayCollisionVolumeComponent() override = default;
};

using RayCollisionVolumeComponentFactory = GenericComponentFactory<RayCollisionVolumeComponent>;

#endif // HEXAGON_CITY_COMPONENTS_RAY_COLLISION_VOLUME_COMPONENT_HPP
