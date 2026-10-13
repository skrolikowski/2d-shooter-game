#include "Input.h"

void Input::OnGamepadAdded(const SDL_JoystickID id)
{
    if (Gamepad) return;
    Gamepad.reset(SDL_OpenGamepad(id));
}

void Input::OnGamepadRemoved(const SDL_JoystickID id)
{
    if (Gamepad && SDL_GetGamepadID(Gamepad.get()) == id)
    {
        Gamepad.reset();
        // SDL_Log("Gamepad #%u removed", (unsigned int) id);
    }
}

void Input::Update()
{
    Prev = Curr;

    const bool* keys = SDL_GetKeyboardState(nullptr);
    for (size_t i = 0; i < ActionCount; ++i)
    {
        bool down = false;
        for (SDL_Scancode k : Bindings[i].keys)
            down |= keys[k];
        for (SDL_GamepadButton b : Bindings[i].buttons)
            down |= Gamepad && SDL_GetGamepadButton(Gamepad.get(), b);
        for (const AxisBinding& ab : Bindings[i].axes)
            down |= Gamepad && SDL_GetGamepadAxis(Gamepad.get(), ab.axis) * ab.sign > AXIS_DEADZONE;
        
        Curr[i] = down;
    }
}