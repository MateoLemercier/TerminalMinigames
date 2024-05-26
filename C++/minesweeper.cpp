#include "minesweeper.h"
#include <iostream>
using namespace std;



Minesweeper::Minesweeper( int iRowCount = 14, int iColumnCount = 18, int iBombCount = 40 )
{
    m_iRowCount = iRowCount;
    m_iColumnCount = iColumnCount;
    m_iBombCount = iBombCount;
    m_pBoard = new MinesweeperCell*[ m_iRowCount ];
    for ( int i = 0; i < m_iRowCount; i++ )
        m_pBoard[ i ] = new MinesweeperCell[ m_iColumnCount ];
    m_iRevealedCount = 0;
    m_iFlaggedCount = 0;
    PrintBoard();
}

Minesweeper::~Minesweeper()
{
    for ( int i = 0; i < m_iRowCount; i++ )
        delete[] m_pBoard[ i ];
    delete[] m_pBoard;
}

void Minesweeper::PrintBoard() const
{
    for ( int iRowIndex = 0; iRowIndex < m_iRowCount; iRowIndex++ )
    {
        for ( int iColumnIndex = 0; iColumnIndex < m_iColumnCount; iColumnIndex++ )
        {
            MinesweeperCell& rCell = m_pBoard[ iRowIndex ][ iColumnIndex ];
            
            if ( rCell.isRevealed == false )
            {
                if ( rCell.isFlagged == true )
                {
                    cout << "\033[4;30;43m  \033[0m ";
                    continue;
                }
                cout << "\033[4;30;42m  \033[0m ";
                continue;
            }
            
            if ( rCell.isMine == true )
            {
                cout << "\033[4;30;41m  \033[0m ";
                continue;
            }
            
            if ( rCell.value == 0 )
            {
                cout << " - ";
                continue;
            }
            
            cout << ' ' << '0' + rCell.value << ' ';
        }
        cout << endl;
    }
}



int main()
{
    Minesweeper();
    while ( true ){};
    return 0;
}