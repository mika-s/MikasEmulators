#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_GUI_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_GUI_H

namespace emu::cpu8080::applications::space_invaders {

    class Gui {
    public:
        virtual ~Gui() = default;

        virtual void update_screen(const std::vector<std::uint8_t> &vram) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_GUI_H
