#ifndef HEXAGON_CITY_HEX_COORDINATES_HPP
#define HEXAGON_CITY_HEX_COORDINATES_HPP

#include <array>
#include <cstdint>
#include <tuple>

using namespace Penrose;

using AxialCoord = std::tuple<std::int32_t, std::int32_t>; // q, r
using CubeCoord = std::tuple<std::int32_t, std::int32_t, std::int32_t>; // q, r, s

template<typename T>
concept HexCoord = std::is_same_v<T, AxialCoord> || std::is_same_v<T, CubeCoord>;

constexpr static const std::array<AxialCoord, 6> AXIAL_DIRECTIONS = {
        std::make_tuple(1, 0),
        std::make_tuple(1, -1),
        std::make_tuple(0, -1),
        std::make_tuple(-1, 0),
        std::make_tuple(-1, +1),
        std::make_tuple(0, +1),
};

constexpr AxialCoord axialAdd(const AxialCoord &hex, const AxialCoord &dir) {
    const auto &[hex_q, hex_r] = hex;
    const auto &[dir_q, dir_r] = dir;

    return {
            hex_q + dir_q,
            hex_r + dir_r
    };
}

constexpr AxialCoord axialNeighbor(const AxialCoord &hex, std::uint32_t dir) {
    return axialAdd(hex, AXIAL_DIRECTIONS.at(dir));
}

constexpr static const std::array<CubeCoord, 6> CUBE_DIRECTIONS = {
        std::make_tuple(1, 0, -1),
        std::make_tuple(1, -1, 0),
        std::make_tuple(0, -1, 1),
        std::make_tuple(-1, 0, 1),
        std::make_tuple(-1, 1, 0),
        std::make_tuple(0, 1, -1),
};

constexpr CubeCoord cubeAdd(const CubeCoord &hex, const CubeCoord &dir) {
    const auto &[hex_q, hex_r, hex_s] = hex;
    const auto &[dir_q, dir_r, dir_s] = dir;

    return {
            hex_q + dir_q,
            hex_r + dir_r,
            hex_s + dir_s
    };
}

constexpr CubeCoord cubeScale(const CubeCoord &hex, std::int32_t scale) {
    const auto &[q, r, s] = hex;

    return {
            scale * q,
            scale * r,
            scale * s
    };
}

constexpr CubeCoord cubeNeighbor(const CubeCoord &hex, std::uint32_t dir) {
    return cubeAdd(hex, CUBE_DIRECTIONS.at(dir));
}

template<HexCoord From, HexCoord To = From>
constexpr To convert(const From &hex) {
    return hex;
}

template<>
constexpr AxialCoord convert<CubeCoord, AxialCoord>(const CubeCoord &hex) {
    const auto &[q, r, s] = hex;

    return {
            q,
            r,
    };
}

#endif // HEXAGON_CITY_HEX_COORDINATES_HPP
