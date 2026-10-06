#ifndef INPUT_H
#define INPUT_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <SDL3/SDL.h>

class Input
{
public:
    static void Init();
    static void Update();

    static bool IsDown(SDL_Scancode key);
    static bool WasDown(SDL_Scancode key);
    static bool IsReleased(SDL_Scancode key);
    static bool IsPressed(SDL_Scancode key);

private:
    static int numKeys;
    static const bool* keyboardState;
    static std::unique_ptr<bool[]> prevKeyboardState;

};

#endif // INPUT_H
