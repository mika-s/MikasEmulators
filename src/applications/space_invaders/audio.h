#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_AUDIO_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_AUDIO_H

#include <cstdint>
#include <SDL.h>
#include <vector>
#include "crosscutting/typedefs.h"

namespace emu::applications::space_invaders {
    class Audio {

    public:
        Audio();

        ~Audio();

        void play_sound_port_1(u8 acc_reg);

        void play_sound_port_2(u8 acc_reg);

        void change_volume(unsigned int new_volume);

        void mute();

        void unmute();

        void toggle_mute();

    private:
        /* Port 3: (discrete sounds)
         *   bit 0 = UFO (repeats)        SX0 0.raw
         *   bit 1 = Shot                 SX1 1.raw
         *   bit 2 = Flash (player die)   SX2 2.raw
         *   bit 3 = Invader die          SX3 3.raw
         *   bit 4 = Extended play        SX4
         *   bit 5 = AMP enable           SX5
         */
        static constexpr unsigned int ufo = 0;
        static constexpr unsigned int shot = 1;
        static constexpr unsigned int flash = 2;
        static constexpr unsigned int invader_die = 3;
        static constexpr unsigned int extended_play = 4;
        static constexpr unsigned int amp_enable = 5;       // not implemented
        static constexpr unsigned int no_sound = 255;

        /* Port 5:
         *   bit 0 = Fleet movement 1     SX6 4.raw
         *   bit 1 = Fleet movement 2     SX7 5.raw
         *   bit 2 = Fleet movement 3     SX8 6.raw
         *   bit 3 = Fleet movement 4     SX9 7.raw
         *   bit 4 = UFO Hit              SX10 8.raw
         */
        static constexpr unsigned int fleet_movement_1 = 0;
        static constexpr unsigned int fleet_movement_2 = 1;
        static constexpr unsigned int fleet_movement_3 = 2;
        static constexpr unsigned int fleet_movement_4 = 3;
        static constexpr unsigned int ufo_hit = 4;

        SDL_AudioDeviceID m_audio_device;
        std::size_t m_ufo_x = 0;
        std::size_t m_shot_x = 0;
        std::size_t m_flash_x = 0;
        std::size_t m_invader_die_x = 0;
        std::size_t m_extended_play_x = 0;

        std::size_t m_fleet_movement_1_x = 0;
        std::size_t m_fleet_movement_2_x = 0;
        std::size_t m_fleet_movement_3_x = 0;
        std::size_t m_fleet_movement_4_x = 0;
        std::size_t m_ufo_hit_x = 0;

        bool m_is_ufo_sound_on;
        bool m_is_shot_sound_on;
        bool m_is_flash_sound_on;
        bool m_is_invader_die_sound_on;
        bool m_is_extended_play_sound_on;
        bool m_is_fleet_movement_1_sound_on;
        bool m_is_fleet_movement_2_sound_on;
        bool m_is_fleet_movement_3_sound_on;
        bool m_is_fleet_movement_4_sound_on;
        bool m_is_ufo_hit_sound_on;

        bool m_is_muted;

        u8 m_last_acc_reg;

        unsigned int m_volume = 5000;

        [[nodiscard]] bool is_rising_edge(u8 acc_reg, unsigned int value) const;

        [[nodiscard]] bool is_falling_edge(u8 acc_reg, unsigned int value) const;

        void generate_audio(Uint8 *stream, int len);

        void play(std::vector<double> sound, int samples, Sint16 *stream16, std::size_t &x, bool &is_sound_on) const;

        static void forward_callback(void *userdata, Uint8 *stream, int len) {
            static_cast<Audio *>(userdata)->generate_audio(stream, len);
        }
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_AUDIO_H
