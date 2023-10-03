#ifndef HEXAGON_CITY_DEBUG_CITY_GENERATION_SYSTEM_HPP
#define HEXAGON_CITY_DEBUG_CITY_GENERATION_SYSTEM_HPP

#include <Penrose/ECS/System.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

using namespace Penrose;

class DebugCityGenerationSystem : public Resource, public Initializable, public System {
public:
    explicit DebugCityGenerationSystem(ResourceSet *resources);
    ~DebugCityGenerationSystem() override = default;

    void init() override;

    void destroy() override { /* nothing to do */ }

    void update(float) override { /* nothing to do */ }

    [[nodiscard]] std::string getName() const override { return "CityGeneration"; }

private:
    Lazy<ECSManager> _ecsManager;
    Lazy<SceneManager> _sceneManager;
};


#endif // HEXAGON_CITY_DEBUG_CITY_GENERATION_SYSTEM_HPP
