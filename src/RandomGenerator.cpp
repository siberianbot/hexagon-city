#include "RandomGenerator.hpp"

#include <stdexcept>

std::uint32_t RandomGenerator::generateInt(std::uint32_t left, std::uint32_t right) {
    if (left >= right) {
        throw std::logic_error("Constraint l < r not satisfied");
    }

    auto dist = 1 + right - left;
    auto value = this->_intDistribution(this->_randomEngine) % dist;

    return left + value;
}

float RandomGenerator::generateFloat(float left, float right) {
    if (left >= right) {
        throw std::logic_error("Constraint l < r not satisfied");
    }

    return left + (right - left) * this->_floatDistribution(this->_randomEngine);
}
