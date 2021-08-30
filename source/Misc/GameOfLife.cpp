#include "GameOfLife.h"

GameOfLife::GameOfLife(int width, int height) : m_boardWidth{ width }, m_boardHeight{ height }
{
    std::vector<bool> zeroRow(width, false);
    for (int rowCounter{ 0 }; rowCounter < height; rowCounter++)
    {
        m_wipedBoard.push_back(zeroRow);
    }

    m_gameBoard = m_wipedBoard;
}

GameOfLife::~GameOfLife()
{}

bool GameOfLife::boardIdxsOutOfBounds(int rowIdx, int colIdx) const
{
    return rowIdx >= m_boardHeight || rowIdx < 0 || colIdx >= m_boardWidth || colIdx < 0;
}

void GameOfLife::setLifeAtPos(bool alive, int rowIdx, int colIdx)
{
    if (boardIdxsOutOfBounds(rowIdx, colIdx))
    {
        std::cout << "Game board index out of bounds!" << std::endl;
        return;
    }

    m_gameBoard[rowIdx][colIdx] = alive;
}

void GameOfLife::updateState()
{
    std::vector<std::vector<bool>> newBoard{ m_wipedBoard };

    for (int rowIdx{ 0 }; rowIdx < m_boardHeight; rowIdx++)
    {
        for (int colIdx{ 0 }; colIdx < m_boardWidth; colIdx++)
        {   
            int liveNeighbours{ 0 };
            int deadNeighbours{ 0 };

            for (int neighbourRowDiff{ -1 }; neighbourRowDiff <= 1; neighbourRowDiff++)
            {
                for (int neighbourColDiff{ -1 }; neighbourColDiff <= 1; neighbourColDiff++)
                {
                    int neighbourRowIdx{ neighbourRowDiff + rowIdx };
                    int neighbourColIdx{ neighbourColDiff + colIdx };
                    if (!(neighbourRowIdx == rowIdx && neighbourColIdx == colIdx) && !boardIdxsOutOfBounds(neighbourRowIdx, neighbourColIdx))
                    {
                        if (m_gameBoard[neighbourRowIdx][neighbourColIdx])
                        {
                            liveNeighbours++;
                        }
                        else
                        {
                            deadNeighbours++;
                        }
                    }
                }
            }

            if ((m_gameBoard[rowIdx][colIdx] && (liveNeighbours == 2 || liveNeighbours == 3))
                || (!m_gameBoard[rowIdx][colIdx] && liveNeighbours == 3))
            {
                newBoard[rowIdx][colIdx] = true;
            }
        }
    }

    m_gameBoard = newBoard;
}

void GameOfLife::randomiseBoard(double liveChance)
{
    resetBoard();

    for (std::vector<bool>& row : m_gameBoard)
    {
        for (int idx{ 0 }; idx < m_boardWidth; idx++)
        {
            if (MTRandom::getRandomDouble(0.0, 1.0) < liveChance)
            {
                row[idx] = true;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const GameOfLife& gol)
{
    for (std::vector<bool> row : gol.getGameBoard())
    {
        for (bool cell : row)
        {
            os << static_cast<int>(cell) << ' ';
        }

        os << std::endl;
    }

    return os;
}