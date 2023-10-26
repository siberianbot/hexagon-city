#ifndef HEXAGON_CITY_CELL_TYPE_HPP
#define HEXAGON_CITY_CELL_TYPE_HPP

#include <cstdint>

enum class CellType : std::uint32_t {
    Concrete,
    Grass,
    Sand,
    Water
};

#endif // HEXAGON_CITY_CELL_TYPE_HPP
