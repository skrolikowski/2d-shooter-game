#include "Input.h"

int Input::numKeys;
const bool* Input::keyboardState;
std::unique_ptr<bool[]> Input::prevKeyboardState;

void Input::Init()
{
    keyboardState = SDL_GetKeyboardState(&numKeys);
    prevKeyboardState = std::make_unique<bool[]>(numKeys);
}

void Input::Update()
{
    std::copy_n(keyboardState, numKeys, prevKeyboardState.get());
}

bool Input::IsDown(SDL_Scancode code)
{
    return keyboardState[code];
}

bool Input::WasDown(SDL_Scancode code)
{
    return prevKeyboardState[code];
}

bool Input::IsReleased(SDL_Scancode code)
{
    return WasDown(code) && !IsDown(code);
}

bool Input::IsPressed(SDL_Scancode code)
{
    return !WasDown(code) && IsDown(code);
}