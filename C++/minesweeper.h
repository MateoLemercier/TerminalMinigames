#pragma once



typedef struct MinesweeperCell
{
    int value = 0;
    bool isRevealed = false;
    bool isFlagged = false;
    bool isMine = false;
} MinesweeperCell;



class Minesweeper
{

public:
    Minesweeper( int iRowCount, int iColumnCount, int iBombCount );
    ~Minesweeper();

protected:
    void PrintBoard() const;

protected:
    int m_iRowCount;
    int m_iColumnCount;
    int m_iBombCount;
    MinesweeperCell** m_pBoard;
    int m_iRevealedCount;
    int m_iFlaggedCount;

};