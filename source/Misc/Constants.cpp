#include "Constants.h"

int g_screenWidth{ 1600 };
int g_screenHeight{ 900 };

SDL_Renderer* g_renderer{ nullptr };
TTF_Font* g_font{ nullptr };

bool Settings::fullscreen;
bool Settings::vSync;
int Settings::masterVol;
int Settings::sfxVol;
int Settings::musicVol;

std::mutex UpdateMutexes::M;
std::mutex UpdateMutexes::N;
std::mutex UpdateMutexes::L;

void UpdateMutexes::lowPrioLock()
{
    L.lock();
    N.lock();
    M.lock();
    N.unlock();
}

void UpdateMutexes::lowPrioUnlock()
{
    M.unlock();
    L.unlock();
}

void UpdateMutexes::highPrioLock()
{
    N.lock();
    M.lock();
    N.unlock();
}

void UpdateMutexes::highPrioUnlock()
{
    M.unlock();
}