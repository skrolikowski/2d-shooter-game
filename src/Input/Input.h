#ifndef INPUT_H
#define INPUT_H

#include <array>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL.h>

#include "Defs.h"
#include "Util/SDLPtr.h"

enum class Action : uint8_t { Left, Right, Fire, Confirm, Back, Count };
constexpr std::size_t ActionCount = static_cast<std::size_t>(Action::Count);

struct AxisBinding
{
    SDL_GamepadAxis axis;
    int sign;
};

struct Binding
{
    std::vector<SDL_Scancode> keys;
    std::vector<SDL_GamepadButton> buttons;
    std::vector<AxisBinding> axes;
};


class Input
{
public:
    static void Update();

    static void OnGamepadAdded(const SDL_JoystickID id);
    static void OnGamepadRemoved(const SDL_JoystickID id);

    static bool IsDown(Action action) { return Curr[static_cast<size_t>(action)]; }
    static bool WasDown(Action action) { return Prev[static_cast<size_t>(action)]; }
    static bool IsReleased(Action action) { return WasDown(action) && !IsDown(action); }
    static bool IsPressed(Action action) { return !WasDown(action) && IsDown(action); }

private:
    inline static std::array<bool, ActionCount> Curr{};
    inline static std::array<bool, ActionCount> Prev{};
    inline static SDL_GamepadPtr Gamepad;

    inline static const std::array<Binding, ActionCount> Bindings = {{
        /* Left    */   { {SDL_SCANCODE_A, SDL_SCANCODE_LEFT},  {SDL_GAMEPAD_BUTTON_DPAD_LEFT},  { {SDL_GAMEPAD_AXIS_LEFTX, -1} } },
        /* Right   */   { {SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, {SDL_GAMEPAD_BUTTON_DPAD_RIGHT}, { {SDL_GAMEPAD_AXIS_LEFTX, +1} } },
        /* Fire    */   { {SDL_SCANCODE_SPACE},                 {SDL_GAMEPAD_BUTTON_SOUTH}, { } },
        /* Confirm */   { {SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN}, {SDL_GAMEPAD_BUTTON_START}, { } },
        /* Back    */   { {SDL_SCANCODE_ESCAPE},                {SDL_GAMEPAD_BUTTON_BACK}, { } },
    }};

};

#endif // INPUT_H
