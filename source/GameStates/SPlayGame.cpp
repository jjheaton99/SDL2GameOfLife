#include "SPlayGame.h"

SPlayGame::SPlayGame()
{
    setBoardSizes();
}

SPlayGame::~SPlayGame()
{}

void SPlayGame::setBoardSizes()
{
    m_cellSize = (g_screenWidth / static_cast<double>(g_screenHeight) > m_gameWidth / static_cast<double>(m_gameHeight))
        ? g_screenHeight / m_gameHeight : g_screenWidth / m_gameWidth;

    m_boardStartX = (g_screenWidth - m_cellSize * m_gameWidth) / 2;
    m_boardStartY = (g_screenHeight - m_cellSize * m_gameHeight) / 2;

    m_boardAreaRect.x = m_boardStartX;
    m_boardAreaRect.y = m_boardStartY;
    m_boardAreaRect.w = m_cellSize * m_gameWidth;
    m_boardAreaRect.h = m_cellSize * m_gameHeight;
}

std::pair<int, int> SPlayGame::getMouseCellPos() const
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);

    x -= m_boardStartX;
    y -= m_boardStartY;

    int xPos{ -1 };
    int yPos{ -1 };
    if ((x >= 0 && x < m_cellSize * m_gameWidth) && (y >= 0 && y < m_cellSize * m_gameHeight))
    {
        xPos = x / m_cellSize;
        yPos = y / m_cellSize;
    }

    return std::make_pair(xPos, yPos);
}

void SPlayGame::playerControlsHold(SDL_GameController* controller)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };
    const Uint32 currentMouseButtonState{ SDL_GetMouseState(nullptr, nullptr) };
    const Sint16 currentLeftyState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) };
    const Sint16 currentLeftxState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) };

    std::pair<int, int> mousePos{ getMouseCellPos() };
    if ((mousePos.first >= 0 && mousePos.first < m_gameWidth) && (mousePos.second >= 0 && mousePos.second < m_gameHeight))
    {
        bool alive{ m_gameOfLife->getGameBoard()[mousePos.second][mousePos.first] };

        if ((currentMouseButtonState & SDL_BUTTON_LMASK) && !alive)
        {
            m_gameOfLife->setLifeAtPos(true, mousePos.second, mousePos.first);
        }

        if ((currentMouseButtonState & SDL_BUTTON_RMASK) && alive)
        {
            m_gameOfLife->setLifeAtPos(false, mousePos.second, mousePos.first);
        }
    }
}

void SPlayGame::playerControlsPress(SDL_Event& event, SDL_GameController* controller)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };
    const Sint16 currentLeftyState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) };
    const Sint16 currentLeftxState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) };

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            m_frozen ? unfreezeGame() : freezeGame();
            break;
        case SDLK_r:
            freezeGame();
            m_gameOfLife->resetBoard();
            break;
        case SDLK_t:
            freezeGame();
            m_gameOfLife->randomiseBoard(0.2);
            break;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        std::pair<int, int> mousePos{ getMouseCellPos() };
        if ((mousePos.first >= 0 && mousePos.first < m_gameWidth) && (mousePos.second >= 0 && mousePos.second < m_gameHeight))
        {
            bool alive{ m_gameOfLife->getGameBoard()[mousePos.second][mousePos.first] };

            if (event.button.button == SDL_BUTTON_LEFT && !alive)
            {
                m_gameOfLife->setLifeAtPos(true, mousePos.second, mousePos.first);
            }

            if (event.button.button == SDL_BUTTON_RIGHT && alive)
            {
                m_gameOfLife->setLifeAtPos(false, mousePos.second, mousePos.first);
            }
        }
    }
}

GameState::State SPlayGame::handleEvents(SDL_GameController* controller)
{
    playerControlsHold(controller);

    for (SDL_Event& element : m_events)
    {
        if ((element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
            || (element.type == SDL_CONTROLLERBUTTONDOWN && element.cbutton.button == SDL_CONTROLLER_BUTTON_START))
        {
            if (m_frozen)
            {
                m_paused = true;
                m_stepTimer.pause();
                return PAUSED;
            }
            else
            {
                m_frozen = true;
            }
        }

        playerControlsPress(element, controller);
    }

    return STATE_NULL;
}

GameState::State SPlayGame::update()
{
    double timeStep = m_stepTimer.getTicks() / 1000.0;

    if (!m_paused && !m_frozen)
    {
        m_timeAccumulator += timeStep;

        if (m_timeAccumulator > Constants::updateStep)
        {
            m_updateTimer.start();

            m_timeAccumulator = 0;

            m_gameOfLife->updateState();
           
            m_updateTimer.stop();
        }

        //std::cout << updateCount << '\n';
        //updateCount = 0;

        m_stepTimer.start();

        m_camera.resize();

        return STATE_NULL;
    }

    m_paused = false;
    m_stepTimer.resume();

    return STATE_NULL;
}

void SPlayGame::render()
{
    setBoardSizes();

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(g_renderer, 0, 0, 20, 255);
    SDL_RenderFillRect(g_renderer, &m_boardAreaRect);

    for (int rowIdx{ 0 }; rowIdx < m_gameHeight; rowIdx++)
    {
        for (int colIdx{ 0 }; colIdx < m_gameWidth; colIdx++)
        {
            UpdateMutexes::highPrioLock();
            bool cellAlive{ m_gameOfLife->getGameBoard()[rowIdx][colIdx] };
            UpdateMutexes::highPrioUnlock();
            if (cellAlive)
            {
                SDL_Rect cell{ m_boardStartX + m_cellSize * colIdx, m_boardStartY + m_cellSize * rowIdx, m_cellSize, m_cellSize };
                SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(g_renderer, &cell);
            }
        }
    }

    if (m_frozen)
    {
        SDL_SetRenderDrawColor(g_renderer, 0, 50, 100, 100);
        SDL_RenderFillRect(g_renderer, &m_boardAreaRect);

        SDL_SetRenderDrawColor(g_renderer, 0, 100, 50, 255);
        for (int rowIdx{ 0 }; rowIdx <= m_gameHeight; rowIdx++)
        {
            SDL_RenderDrawLine(g_renderer, m_boardStartX, m_boardStartY + rowIdx * m_cellSize,
                m_boardStartX + m_gameWidth * m_cellSize, m_boardStartY + rowIdx * m_cellSize);
        }
        for (int colIdx{ 0 }; colIdx <= m_gameWidth; colIdx++)
        {
            SDL_RenderDrawLine(g_renderer, m_boardStartX + colIdx * m_cellSize, m_boardStartY,
                m_boardStartX + colIdx * m_cellSize, m_boardStartY + m_gameHeight * m_cellSize);
        }
    }
}

double SPlayGame::averageFPS(double timeStep)
{
    m_FPSTime += timeStep;
    ++m_FPSCount;
    if (m_FPSTime > 0.1)
    {
        double averageFPS{ m_FPSCount / m_FPSTime };
        m_FPSCount = 0;
        m_FPSTime = 0.0;
        return averageFPS;
    }

    return -1.0;
}
