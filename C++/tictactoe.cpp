#include "tictactoe.h"
#include <iostream>
#include <conio.h>
using namespace std;
using namespace TicTacToe;

int Find( const int iInt, const int* const pList )
{
    for( int i = 0; i < sizeof( pList ); i++ )
        if( pList[i] == iInt )
            return i;
    return -1;
}

int GetKeyFromList( const int* const pList )
{
    int iKey;
    while( true )
    {
        if ( kbhit() )
        {
            iKey = getch();
            if ( Find( iKey, pList ) != -1 )
                return iKey;
        }
    }
}



Game::Game( const int iRowCount = 3, const int iColumnCount = 3, const int iWinStreak = 3 )
{
    m_pBoardSize[0] = iRowCount;
    m_pBoardSize[1] = iColumnCount;
    m_iWinStreak = iWinStreak;
    
    m_pBoard = new Cell*[ m_pBoardSize[0] ];
    for( int iCellIndex = 0; iCellIndex < m_pBoardSize[0]; iCellIndex++ )
        m_pBoard[ iCellIndex ] = new Cell[ m_pBoardSize[1] ];
    
    m_pSelectedCell[0] = ( m_pBoardSize[0] - 1 ) / 2;
    m_pSelectedCell[1] = ( m_pBoardSize[1] - 1 ) / 2;
    
    m_iPlayer = 1;
    
    m_isEndgame = false;
    GameLoop();
}

Game::~Game()
{
    for ( int iCellIndex = 0; iCellIndex < m_pBoardSize[0]; iCellIndex++ ) delete[] m_pBoard[ iCellIndex ];
    delete[] m_pBoard;
}

void Game::MoveSelected( const int iAxis, const int iDistance )
{
    if ( 0 <= m_pSelectedCell[ iAxis ] + iDistance && m_pSelectedCell[ iAxis ] + iDistance < m_pBoardSize[ iAxis ] )
        m_pSelectedCell[ iAxis ] += iDistance;
}

void Game::GameLoop()
{
    int iKey; int pKeys[] = { KEY_ENTER, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW };
    while ( m_isEndgame == false )
    {
        PrintBoard();
        
        iKey = GetKeyFromList( pKeys );
        
        if ( iKey == KEY_UP_ARROW ) MoveSelected( 0, -1 );
        else if ( iKey == KEY_LEFT_ARROW ) MoveSelected( 1, -1 );
        else if ( iKey == KEY_RIGHT_ARROW ) MoveSelected( 1, 1 );
        else if ( iKey == KEY_DOWN_ARROW ) MoveSelected( 0, 1 );
        else if ( iKey == KEY_ENTER ) ;
    }
    PrintBoard();
}