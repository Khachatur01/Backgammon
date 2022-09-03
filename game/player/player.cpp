#include <cstdint>
#include <string>
#include <utility>
#include "player.h"
/* private */

/* public */
Player::Player(uint8_t pips_count, std::string peace) {
    this->peaces = new uint8_t[pips_count]{0};
    this->peaces[0] = this->PEACES_COUNT;
    this->peace = std::move(peace);
}