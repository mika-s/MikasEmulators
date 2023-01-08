#pragma once

#include <string>
#include <unordered_map>

namespace emu::applications::zxspectrum_48k {

enum class JoystickType {
    Cursor_Protek_AGF,
    Kempston,
    Sinclair_2_Left,
    Sinclair_2_Right,
    UserDefined
};

static const inline std::unordered_map<JoystickType, std::string> s_joystick_type_as_string = {
    { JoystickType::Cursor_Protek_AGF, "Cursor/Protek/AGF joystick" },
    { JoystickType::Kempston, "Kempston joystick" },
    { JoystickType::Sinclair_2_Left, "Sinclair 2 Left joystick" },
    { JoystickType::Sinclair_2_Right, "Sinclair 2 Right joystick" },
    { JoystickType::UserDefined, "User-defined joystick" }
};

}
