#ifndef HEXAGON_CITY_RAY_COLLISION_VOLUME_COMPONENT_HPP
#define HEXAGON_CITY_RAY_COLLISION_VOLUME_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class RayCollisionVolumeComponent : public Component {
public:
    ~RayCollisionVolumeComponent() override = default;

    [[nodiscard]] std::string getName() const override { return name(); }

    [[nodiscard]] constexpr static std::string name() { return "RayCollisionVolume"; }
};

class RayCollisionVolumeComponentFactory : public Resource<RayCollisionVolumeComponentFactory>,
                                           public GenericComponentFactory<RayCollisionVolumeComponent> {
    //
};

#endif // HEXAGON_CITY_RAY_COLLISION_VOLUME_COMPONENT_HPP
