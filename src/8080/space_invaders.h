#ifndef MIKA_EMULATORS_8080_SPACE_INVADERS_H
#define MIKA_EMULATORS_8080_SPACE_INVADERS_H

#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_video.h>
#include <cstdint>
#include <vector>
#include <memory>
#include "cpu.h"
#include "emulator8080.h"
#include "in_observer.h"
#include "instructions.h"
#include "out_observer.h"
#include "shift_register.h"

namespace emu::cpu8080 {

    enum NumberOfLives {
        Three = 0, Four = 1, Five = 2, Six = 3
    };

    enum BonusLifeAt {
        _1000 = 1, _1500 = 0
    };

    enum CoinInfo {
        On = 0, Off = 1
    };

    class SpaceInvadersSettings {
    public:
        NumberOfLives number_of_lives;
        BonusLifeAt bonus_life_at;
        CoinInfo coin_info;
    };

    class SpaceInvadersIo {
    public:
        SpaceInvadersIo(std::uint8_t in_port0, std::uint8_t in_port1, std::uint8_t in_port2);

        void set_dipswitches(const SpaceInvadersSettings &settings);

        std::uint8_t in_port0;
        std::uint8_t in_port1;
        std::uint8_t in_port2;
        ShiftRegister shift_register;

    private:
        static constexpr int dipswitch_bonus_life = 3;
        static constexpr int dipswitch_coin_info = 7;
    };

    class SpaceInvadersGui {
    public:
        SpaceInvadersGui();

        ~SpaceInvadersGui();

        void init();

        void update_screen(const std::vector<std::uint8_t> &vram);

    private:
        static constexpr float scale = 4.0;
        static constexpr int width = 224;
        static constexpr int height = 256;
        static constexpr int colors = 3;
        static constexpr int bits_in_byte = 8;
        static constexpr int scaled_width = (int)(scale * (float)width);
        static constexpr int scaled_height = (int)(scale * (float)height);

        SDL_Window *win;
        SDL_Renderer *rend;
        SDL_Texture *texture;
    };

    class SpaceInvadersInput {
    public:
        static void read(bool &is_finished, bool &is_paused, SpaceInvadersIo &cpu_io);

    private:
        static constexpr SDL_Scancode pause = SDL_SCANCODE_PAUSE;

        static constexpr SDL_Scancode insert_coin = SDL_SCANCODE_C;
        static constexpr SDL_Scancode tilt = SDL_SCANCODE_T;

        static constexpr SDL_Scancode p1_start = SDL_SCANCODE_RETURN;
        static constexpr SDL_Scancode p1_shoot = SDL_SCANCODE_W;
        static constexpr SDL_Scancode p1_left = SDL_SCANCODE_A;
        static constexpr SDL_Scancode p1_right = SDL_SCANCODE_D;

        static constexpr SDL_Scancode p2_start = SDL_SCANCODE_RSHIFT;
        static constexpr SDL_Scancode p2_shoot = SDL_SCANCODE_UP;
        static constexpr SDL_Scancode p2_left = SDL_SCANCODE_LEFT;
        static constexpr SDL_Scancode p2_right = SDL_SCANCODE_RIGHT;
    };

    class SpaceInvaders: public Emulator8080, public OutObserver, public InObserver {
    public:
        explicit SpaceInvaders(const SpaceInvadersSettings &settings);

        void run() override;

        void in_requested(std::uint8_t port) override;

        void out_changed(std::uint8_t port) override;

    private:
        bool is_paused;

        SpaceInvadersIo cpu_io;
        SpaceInvadersGui gui;

        std::unique_ptr<Cpu> cpu;

        EmulatorMemory memory;

        // Game loop - begin
        static constexpr double fps = 60.0;
        static constexpr long double tick_limit = 1000.0 / fps;
        static constexpr int cycles_per_ms = 2000;
        static constexpr long double cycles_per_tick = cycles_per_ms * tick_limit;
        // Game loop - end

        // IO - begin
        static constexpr int in_port_unused = 0;
        static constexpr int in_port_1 = 1;
        static constexpr int in_port_2 = 2;
        static constexpr int in_port_read_shift = 3;

        static constexpr int out_port_shift_offset = 2;
        static constexpr int out_port_sound_1 = 3;
        static constexpr int out_port_do_shift = 4;
        static constexpr int out_port_sound_2 = 5;
        static constexpr int out_port_watchdog = 6;
        // IO - end

        void load_file();

        std::vector<std::uint8_t> vram();

        static std::vector<uint8_t> create_work_ram();

        static std::vector<uint8_t> create_vram();
    };
}

#endif //MIKA_EMULATORS_8080_SPACE_INVADERS_H
