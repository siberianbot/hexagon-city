#ifndef HEXAGON_CITY_GRID_CONSTANTS_HPP
#define HEXAGON_CITY_GRID_CONSTANTS_HPP

#include <array>

#include <glm/vec3.hpp>

constexpr glm::vec3 rgb(double r, double g, double b) {
    return {
            r / 255,
            g / 255,
            b / 255
    };
}

constexpr static const glm::vec3 GRID_HOVERED_COLOR = rgb(200, 200, 50);
constexpr static const glm::vec3 GRID_SELECTED_COLOR = rgb(150, 150, 50);

constexpr static const std::array<glm::vec3, 3> GRID_BUILDING_COLORS = {
        rgb(200, 255, 200), // residential
        rgb(255, 200, 200), // industrial
        rgb(200, 200, 255), // commercial
};

constexpr static const double GRID_BUILDING_PER_LEVEL_HEIGHT = 1.0;

constexpr static const std::uint8_t GRID_BUILDING_MAX_LEVEL = 3;

constexpr static const std::array<glm::vec3, 4> GRID_CELL_COLORS = {
        rgb(224, 224, 224), // concrete
        rgb(33, 207, 97),   // grass
        rgb(227, 216, 159), // sand
        rgb(56, 169, 217),  // water
};
constexpr static const double GRID_CELL_HEIGHT = 0.01;

constexpr static const float GRID_CELL_RADIUS = 0.5f;
constexpr static const float GRID_CELL_DIAMETER = 2 * GRID_CELL_RADIUS;

#endif // HEXAGON_CITY_GRID_CONSTANTS_HPP
