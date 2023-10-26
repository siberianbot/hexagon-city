#ifndef HEXAGON_CITY_PLAYER_STATE_CONTEXT_HPP
#define HEXAGON_CITY_PLAYER_STATE_CONTEXT_HPP

#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

class PlayerStateContext : public Resource<PlayerStateContext, ResourceGroup::Custom> {
public:
    ~PlayerStateContext() override = default;

    [[nodiscard]] std::uint32_t &balance() { return this->_balance; }

private:
    std::uint32_t _balance = 10000;
};

#endif // HEXAGON_CITY_PLAYER_STATE_CONTEXT_HPP
