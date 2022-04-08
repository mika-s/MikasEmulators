#include "audio.h"
#include "crosscutting/byte_util.h"

using emu::util::byte::is_bit_set;

namespace emu::cpu8080::applications::space_invaders {

    void Audio::play_sound_port_1(std::uint8_t acc_reg) {
        if (is_bit_set(acc_reg, ufo)) {
        }
        if (is_bit_set(acc_reg, shot)) {
        }
        if (is_bit_set(acc_reg, flash)) {
        }
        if (is_bit_set(acc_reg, invader_die)) {
        }
        if (is_bit_set(acc_reg, extended_play)) {
        }
        if (is_bit_set(acc_reg, amp_enable)) {
        }
    }

    void Audio::play_sound_port_2(std::uint8_t acc_reg) {
        if (is_bit_set(acc_reg, fleet_movement_1)) {
        }
        if (is_bit_set(acc_reg, fleet_movement_2)) {
        }
        if (is_bit_set(acc_reg, fleet_movement_3)) {
        }
        if (is_bit_set(acc_reg, fleet_movement_4)) {
        }
        if (is_bit_set(acc_reg, ufo_hit)) {
        }
    }
}
