#pragma once

#include "random.h"
#include <vector>
#include <iostream>

class GameOfLife
{
private:
    std::vector<std::vector<bool>> m_gameBoard{};
    const int m_boardWidth;
    const int m_boardHeight;
    std::vector<std::vector<bool>> m_wipedBoard{};

    bool boardIdxsOutOfBounds(int row_idx, int col_idx) const;

public:
    GameOfLife(int width = 160, int height = 90);
    ~GameOfLife();

    const std::vector<std::vector<bool>>& getGameBoard() const { return m_gameBoard; }
    const int gameWidth() const { return m_boardWidth; }
    const int gameHeight() const { return m_boardHeight; }

    void setLifeAtPos(bool alive, int row_idx, int col_idx);
    void updateState();

    void resetBoard() { m_gameBoard = m_wipedBoard; }
    void randomiseBoard(double liveChance = 0.5);

    friend std::ostream& operator<<(std::ostream& os, const GameOfLife& gol);
};