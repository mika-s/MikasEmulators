#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_AUDIO_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_AUDIO_H

#include <cstdint>

namespace emu::cpu8080::applications::space_invaders {
    class Audio {

    public:
        static void play_sound_port_1(std::uint8_t acc_reg);

        static void play_sound_port_2(std::uint8_t acc_reg);

    private:

        /* Port 3: (discrete sounds)
         *   bit 0 = UFO (repeats)        SX0 0.raw
         *   bit 1 = Shot                 SX1 1.raw
         *   bit 2 = Flash (player die)   SX2 2.raw
         *   bit 3 = Invader die          SX3 3.raw
         *   bit 4 = Extended play        SX4
         *   bit 5 = AMP enable           SX5
         *   bit 6 = NC (not wired)
         *   bit 7 = NC (not wired)
         */
        static constexpr unsigned int ufo = 0;
        static constexpr unsigned int shot = 1;
        static constexpr unsigned int flash = 2;
        static constexpr unsigned int invader_die = 3;
        static constexpr unsigned int extended_play = 4;
        static constexpr unsigned int amp_enable = 5;

        /* Port 5:
         *   bit 0 = Fleet movement 1     SX6 4.raw
         *   bit 1 = Fleet movement 2     SX7 5.raw
         *   bit 2 = Fleet movement 3     SX8 6.raw
         *   bit 3 = Fleet movement 4     SX9 7.raw
         *   bit 4 = UFO Hit              SX10 8.raw
         *   bit 5 = NC (Cocktail mode control ... to flip screen)
         *   bit 6 = NC (not wired)
         *   bit 7 = NC (not wired)
         */
        static constexpr unsigned int fleet_movement_1 = 0;
        static constexpr unsigned int fleet_movement_2 = 1;
        static constexpr unsigned int fleet_movement_3 = 2;
        static constexpr unsigned int fleet_movement_4 = 3;
        static constexpr unsigned int ufo_hit = 4;
    };

}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_AUDIO_H
