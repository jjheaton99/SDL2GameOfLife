#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <mutex>

namespace Constants
{
    const double pi{ 3.14159265359 };
    //g defines acceleration due to gravity for game objects
    const double g{ 1.5 };
    //the time between game logic updates
    const double updateStep{ 1.0 / 3.0 };
}

//global namespace for settings variables
namespace Settings
{
    extern bool fullscreen;
    extern bool vSync;
    extern int masterVol;
    extern int sfxVol;
    extern int musicVol;
}

namespace UpdateMutexes
{
    extern std::mutex M;
    extern std::mutex N;
    extern std::mutex L;

    void lowPrioLock();
    void lowPrioUnlock();
    void highPrioLock();
    void highPrioUnlock();
}

extern int g_screenWidth;
extern int g_screenHeight;
//convenient to have a global renderer that any object can use
extern SDL_Renderer* g_renderer;
extern TTF_Font* g_font;


