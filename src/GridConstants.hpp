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

constexpr static const std::array<glm::vec3, 4> GRID_CELL_COLORS = {
        rgb(224, 224, 224), // concrete
        rgb(33, 207, 97),   // grass
        rgb(227, 216, 159), // sand
        rgb(56, 169, 217),  // water
};

constexpr static const glm::vec3 GRID_CELL_SELECTED_COLOR = rgb(200, 50, 50);

#endif // HEXAGON_CITY_GRID_CONSTANTS_HPP
