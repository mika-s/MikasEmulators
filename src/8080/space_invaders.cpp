#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <byte_util.h>
#include "cpu.h"
#include "instructions.h"
#include "space_invaders.h"
#include "file_util.h"

namespace emu::cpu8080 {

    using emu::util::file::read_file_into_vector;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;
    using emu::util::byte::unset_bit;

    SpaceInvaders::SpaceInvaders(const SpaceInvadersSettings &settings)
            : is_paused(false),
              cpu_io(SpaceInvadersIo(0, 0b00001000, 0)) {
        load_file();
        cpu_io.set_dipswitches(settings);
    }

    void SpaceInvaders::load_file() {
        // Memory map:
        memory.add(read_file_into_vector("invaders.h")); // $0000-$07ff: invaders.h
        memory.add(read_file_into_vector("invaders.g")); // $0800-$0fff: invaders.g
        memory.add(read_file_into_vector("invaders.f")); // $1000-$17ff: invaders.f
        memory.add(read_file_into_vector("invaders.e")); // $1800-$1fff: invaders.e
        memory.add(create_work_ram());                   // $2000-$23ff: work RAM
        memory.add(create_vram());                       // $2400-$3fff: video RAM
        memory.add_link(0x2000, 0x4000);                 // $4000-: RAM mirror
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);

        const std::uint16_t initial_PC = 0;

        cpu = std::make_unique<Cpu>(memory, initial_PC);

        cpu->add_out_observer(*this);
        cpu->add_in_observer(*this);
    }

    void SpaceInvaders::in_requested(std::uint8_t port) {
        switch (port) {
            case in_port_unused:
                cpu->input(in_port_unused, cpu_io.in_port0);
                break;
            case in_port_1:
                cpu->input(in_port_1, cpu_io.in_port1);
                break;
            case in_port_2:
                cpu->input(in_port_2, cpu_io.in_port2);
                break;
            case in_port_read_shift:
                cpu->input(in_port_read_shift, cpu_io.shift_register.read());
                break;
            default:
                throw std::runtime_error("Illegal input port for Space Invaders");
        }
    }

    void SpaceInvaders::out_changed(std::uint8_t port) {
        switch (port) {
            case out_port_shift_offset:
                cpu_io.shift_register.change_offset(cpu->A());
                break;
            case out_port_sound_1: {
                /*
                 * Port 3: (discrete sounds)
                 *   bit 0=UFO (repeats)        SX0 0.raw
                 *   bit 1=Shot                 SX1 1.raw
                 *   bit 2=Flash (player die)   SX2 2.raw
                 *   bit 3=Invader die          SX3 3.raw
                 *   bit 4=Extended play        SX4
                 *   bit 5= AMP enable          SX5
                 *   bit 6= NC (not wired)
                 *   bit 7= NC (not wired)
                 *   Port 4: (discrete sounds)
                 *   bit 0-7 shift data (LSB on 1st write, MSB on 2nd)
                 */
                if (cpu->A() & (1 << 3)) {  // invader die
                }

                break;
            }
            case out_port_do_shift:
                cpu_io.shift_register.shift(cpu->A());
                break;
            case out_port_sound_2:
                break;
            case out_port_watchdog:
                break;
            default:
                throw std::runtime_error("Illegal output port for Space Invaders");
        }
    }

    void SpaceInvaders::run() {
        gui.init();

        cpu->start();

        bool is_finished = false;
        unsigned long i;

        std::uint64_t last_tick = SDL_GetTicks64();
        while (!is_finished) {
            if (SDL_GetTicks64() - last_tick >= tick_limit) {
                last_tick = SDL_GetTicks();

                i = 0;
                while (i < static_cast<long>(cycles_per_tick / 2)) {
                    i += cpu->next_instruction();
                }

                if (cpu->is_inta()) {
                    cpu->interrupt(RST_1);
                }

                i = 0;
                while (i < static_cast<long>(cycles_per_tick / 2)) {
                    i += cpu->next_instruction();
                }

                SpaceInvadersInput::read(is_finished, is_paused, cpu_io);
                gui.update_screen(this->vram());

                if (cpu->is_inta()) {
                    cpu->interrupt(RST_2);
                }
            }
        }
    }

    std::vector<std::uint8_t> SpaceInvaders::create_work_ram() {
        std::vector<std::uint8_t> work_ram;
        const int size = 1024;

        work_ram.reserve(size);
        for (int i = 0; i < size; ++i) {
            work_ram.push_back(0);
        }

        return work_ram;
    }

    std::vector<std::uint8_t> SpaceInvaders::create_vram() {
        std::vector<std::uint8_t> vram;
        const int size = 7168;

        vram.reserve(size);
        for (int i = 0; i < size + 1; ++i) {
            vram.push_back(0);
        }

        return vram;
    }

    std::vector<std::uint8_t> SpaceInvaders::vram() {
        return {memory.begin() + 0x2400, memory.begin() + 0x3fff};
    }

    SpaceInvadersIo::SpaceInvadersIo(
            std::uint8_t in_port0,
            std::uint8_t in_port1,
            std::uint8_t in_port2
    )
            : in_port0(in_port0),
              in_port1(in_port1),
              in_port2(in_port2) {
    }

    void SpaceInvadersIo::set_dipswitches(const SpaceInvadersSettings &settings) {
        if (settings.coin_info == CoinInfo::Off) {
            in_port2 |= (1U << dipswitch_coin_info);
        }

        if (settings.bonus_life_at == BonusLifeAt::_1000) {
            in_port2 |= (1U << dipswitch_bonus_life);
        }

        switch (settings.number_of_lives) {
            case NumberOfLives::Six:
                in_port2 |= 0b00000011;
                break;
            case NumberOfLives::Five:
                in_port2 |= 0b00000010;
                break;
            case NumberOfLives::Four:
                in_port2 |= 0b00000001;
                break;
            case NumberOfLives::Three:
            default:
                break;
        }
    }

    SpaceInvadersGui::SpaceInvadersGui()
            : win(nullptr),
              rend(nullptr),
              texture(nullptr) {
    }

    SpaceInvadersGui::~SpaceInvadersGui() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();

        texture = nullptr;
        rend = nullptr;
        win = nullptr;
    }

    void SpaceInvadersGui::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        win = SDL_CreateWindow(
                "Space Invaders",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                scaled_width,
                scaled_height,
                SDL_WINDOW_RESIZABLE
        );

        if (!win) {
            std::cerr << "error creating SDL window: " << SDL_GetError() << "\n";
            exit(1);
        }

        rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

        if (!rend) {
            std::cerr << "error creating SDL renderer: " << SDL_GetError() << "\n";
            exit(1);
        }

        if (SDL_RenderSetScale(rend, scale, scale) != 0) {
            std::cerr << "error setting renderer scale in SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32,
                                    SDL_TEXTUREACCESS_STREAMING, width, height);

        if (!texture) {
            std::cerr << "error creating SDL texture: " << SDL_GetError() << "\n";
            exit(1);
        }
    }

    void SpaceInvadersGui::update_screen(const std::vector<std::uint8_t> &vram) {
        std::uint8_t screen[height][width][colors + 1];

        for (int i = 0; i < height * width / bits_in_byte; i++) {
            const int y = i * bits_in_byte / height;
            const int base_x = (i * bits_in_byte) % height;
            const std::uint8_t current_byte = vram[i];

            for (std::uint8_t bit = 0; bit < bits_in_byte; bit++) {
                int px = base_x + bit;
                int py = y;
                const bool is_pixel_lit = is_bit_set(current_byte, bit);
                std::uint8_t r = 0;
                std::uint8_t g = 0;
                std::uint8_t b = 0;

                if (is_pixel_lit) {
                    if (px < 16) {
                        if (py < 16 || 134 < py) {
                            r = 255;
                            g = 255;
                            b = 255;
                        } else {
                            g = 255;
                        }
                    } else if (16 <= px && px <= 72) {
                        g = 255;
                    } else if (192 <= px && px < 224) {
                        r = 255;
                    } else {
                        r = 255;
                        g = 255;
                        b = 255;
                    }
                }

                const int temp_x = px;
                px = py;
                py = -temp_x + height - 1;

                screen[py][px][0] = r;
                screen[py][px][1] = g;
                screen[py][px][2] = b;
            }
        }

        void *pixels = nullptr;
        int pitch = 0;

        if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
            std::cerr << "error while unlocking SDL texture: " << SDL_GetError() << "\n";
        } else {
            memcpy(pixels, screen, pitch * height);
        }

        SDL_UnlockTexture(texture);

        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, texture, nullptr, nullptr);
        SDL_RenderPresent(rend);
    }

    // Ports:
    //	Read 1
    //	BIT	0	coin (0 when active)
    //		1	P2 start button
    //		2	P1 start button
    //		3	?
    //		4	P1 shoot button
    //		5	P1 joystick left
    //		6	P1 joystick right
    //		7	?
    //
    //	Read 2
    //	BIT	0,1	dipswitch number of lives (0:3,1:4,2:5,3:6)
    //		2	tilt 'button'
    //		3	dipswitch bonus life at 1:1000,0:1500
    //		4	P2 shoot button
    //		5	P2 joystick left
    //		6	P2 joystick right
    //		7	dipswitch coin info 1:off,0:on
    void SpaceInvadersInput::read(bool &is_finished, bool &is_paused, SpaceInvadersIo &cpu_io) {
        SDL_Event read_input_event;

        while (SDL_PollEvent(&read_input_event) != 0) {
            switch (read_input_event.type) {

                case SDL_QUIT:
                    is_finished = true;
                    break;

                case SDL_KEYUP:
                    switch (read_input_event.key.keysym.scancode) {
                        case insert_coin:
                            unset_bit(cpu_io.in_port1, 0);
                            break;
                        case tilt:
                            unset_bit(cpu_io.in_port2, 2);
                            break;
                        case p1_start:
                            unset_bit(cpu_io.in_port1, 2);
                            break;
                        case p1_shoot:
                            unset_bit(cpu_io.in_port1, 4);
                            break;
                        case p1_left:
                            unset_bit(cpu_io.in_port1, 5);
                            break;
                        case p1_right:
                            unset_bit(cpu_io.in_port1, 6);
                            break;
                        case p2_start:
                            unset_bit(cpu_io.in_port1, 1);
                            break;
                        case p2_shoot:
                            unset_bit(cpu_io.in_port2, 4);
                            break;
                        case p2_left:
                            unset_bit(cpu_io.in_port2, 5);
                            break;
                        case p2_right:
                            unset_bit(cpu_io.in_port2, 6);
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (read_input_event.key.keysym.scancode) {
                        case pause:
                            is_paused = !is_paused;
                            break;
                        case insert_coin:
                            set_bit(cpu_io.in_port1, 0);
                            break;
                        case tilt:
                            set_bit(cpu_io.in_port2, 2);
                            break;
                        case p1_start:
                            set_bit(cpu_io.in_port1, 2);
                            break;
                        case p1_shoot:
                            set_bit(cpu_io.in_port1, 4);
                            break;
                        case p1_left:
                            set_bit(cpu_io.in_port1, 5);
                            break;
                        case p1_right:
                            set_bit(cpu_io.in_port1, 6);
                            break;
                        case p2_start:
                            set_bit(cpu_io.in_port1, 1);
                            break;
                        case p2_shoot:
                            set_bit(cpu_io.in_port2, 4);
                            break;
                        case p2_left:
                            set_bit(cpu_io.in_port2, 5);
                            break;
                        case p2_right:
                            set_bit(cpu_io.in_port2, 6);
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
    }
}
