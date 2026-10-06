#include "Defs.h"
#include "App.h"

int main()
{
    WindowParams params {
        .title = "2d-shooter-game",
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
    };
    
    App app;
    app.Init(params);
}