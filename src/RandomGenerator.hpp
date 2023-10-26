#ifndef HEXAGON_CITY_RANDOM_GENERATOR_HPP
#define HEXAGON_CITY_RANDOM_GENERATOR_HPP

#include <cstdint>
#include <numeric>
#include <random>

#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class RandomGenerator : public Resource<RandomGenerator, ResourceGroup::Custom> {
public:
    ~RandomGenerator() override = default;

    [[nodiscard]] std::uint32_t generateInt(std::uint32_t left, std::uint32_t right);
    [[nodiscard]] float generateFloat(float left, float right);

private:
    std::random_device _randomDevice = std::random_device();
    std::default_random_engine _randomEngine = std::default_random_engine(this->_randomEngine());

    std::uniform_int_distribution<std::uint32_t> _intDistribution = std::uniform_int_distribution(
            std::numeric_limits<std::uint32_t>::min(),
            std::numeric_limits<std::uint32_t>::max());

    std::uniform_real_distribution<float> _floatDistribution = std::uniform_real_distribution<float>(0, 1);
};

#endif // HEXAGON_CITY_RANDOM_GENERATOR_HPP
