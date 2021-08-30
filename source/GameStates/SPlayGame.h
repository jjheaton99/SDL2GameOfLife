#pragma once

#include "GameState.h"
#include "GameOfLife.h"
#include "Camera.h"
#include "WTimer.h"
#include <vector>
#include <memory>

//main gameplay state
class SPlayGame : public GameState
{
private:
    const int m_gameWidth{ 160 };
    const int m_gameHeight{ 90 };

    int m_cellSize;

    int m_boardStartX;
    int m_boardStartY;

    SDL_Rect m_boardAreaRect;

    std::unique_ptr<GameOfLife> m_gameOfLife{ std::make_unique<GameOfLife>(m_gameWidth, m_gameHeight) };

    void setBoardSizes();

    Camera m_camera{};
    WTimer m_stepTimer;
    WTimer m_updateTimer;
    double m_timeAccumulator{ 0.0 };

    double m_FPSTime{ 0.0 };
    int m_FPSCount{ 0 };

    void playerControlsHold(SDL_GameController* controller);
    void playerControlsPress(SDL_Event& event, SDL_GameController* controller);
    bool m_bumperControllerConfig{ true };
    std::pair<int, int> getMouseCellPos() const;

    double averageFPS(double timeStep);

    bool m_paused{ false };
    bool m_frozen{ true };

public:
    SPlayGame();
    ~SPlayGame();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();

    void pauseGame() { m_paused = true; }
    void resumeGame() { m_paused = false; };
    void freezeGame() { m_frozen = true; }
    void unfreezeGame() { m_frozen = false; }
};

