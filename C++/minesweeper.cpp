#include "minesweeper.h"
#include <iostream>
#include <conio.h>
using namespace std;
using namespace Minesweeper;

//rejouer sur la map

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



Game::Game( const int iRowCount = 14, const int iColumnCount = 18, const int iBombCount = 40 )
{
    m_pBoardSize[0] = iRowCount;
    m_pBoardSize[1] = iColumnCount;
    m_iBombCount = iBombCount;
    m_pBoard = new Cell*[ m_pBoardSize[0] ];
    for ( int i = 0; i < m_pBoardSize[0]; i++ )
        m_pBoard[i] = new Cell[ m_pBoardSize[1] ];
    m_pSelectedCell[0] = 0;
    m_pSelectedCell[1] = 0;
    m_iRevealedCount = 0;
    m_iFlaggedCount = 0;
    m_isEndgame = false;
    srand( time( NULL ) );
    GameLoop();
}

Game::~Game()
{
    for ( int i = 0; i < m_pBoardSize[0]; i++ )
        delete[] m_pBoard[i];
    delete[] m_pBoard;
}

void Game::MoveSelected( const int iAxis, const int iDistance )
{
    if ( 0 <= m_pSelectedCell[ iAxis ] + iDistance && m_pSelectedCell[ iAxis ] + iDistance < m_pBoardSize[ iAxis ] )
        m_pSelectedCell[ iAxis ] += iDistance;
}

void Game::PlaceFlag()
{
    Cell& rSelectedCell = m_pBoard[ m_pSelectedCell[0] ][ m_pSelectedCell[1] ];
    if ( rSelectedCell.isRevealed == true ) return;
    
    bool isFlagged = rSelectedCell.isFlagged;
    
    if ( isFlagged == true )
    {
        rSelectedCell.isFlagged = false;
        m_iFlaggedCount--;
        return;
    }
    
    if ( m_iFlaggedCount < m_iBombCount )
    {
        rSelectedCell.isFlagged = true;
        m_iFlaggedCount++;
    }
}

void Game::GetLimits( Limits& limits, const int iRowIndex, const int iColumnIndex )
{
    if ( iRowIndex - 1 < 0 ) limits.iRowStart = iRowIndex;
    else limits.iRowStart = iRowIndex - 1;
    
    if ( iColumnIndex - 1 < 0 ) limits.iColumnStart = iColumnIndex;
    else limits.iColumnStart = iColumnIndex - 1;
    
    if ( iRowIndex + 1 >= m_pBoardSize[0] ) limits.iRowEnd = iRowIndex;
    else limits.iRowEnd = iRowIndex + 1;
    
    if ( iColumnIndex + 1 >= m_pBoardSize[1] ) limits.iColumnEnd = iColumnIndex;
    else limits.iColumnEnd = iColumnIndex + 1;
}

void Game::PlaceBombs()
{
    int iBoardSize = m_pBoardSize[0] * m_pBoardSize[1];
    int** pPossibleIndex = new int*[ iBoardSize ];
    int iRemovedCount = 0;
    int iRowIndex; int iColumnIndex;
    for ( int i = 0; i < iBoardSize; i++ )
    {
        iRowIndex = i / m_pBoardSize[1];
        iColumnIndex = i % m_pBoardSize[1];
        if ( m_pSelectedCell[0] - 1 <= iRowIndex && iRowIndex <= m_pSelectedCell[0] + 1 && m_pSelectedCell[1] - 1 <= iColumnIndex && iColumnIndex <= m_pSelectedCell[1] + 1 )
        {
            iRemovedCount++;
            continue;
        }
        pPossibleIndex[ i - iRemovedCount ] = new int[2];
        pPossibleIndex[ i - iRemovedCount ][0] = iRowIndex;
        pPossibleIndex[ i - iRemovedCount ][1] = iColumnIndex;
    }
    
    int* pCellIndex;
    Limits limits;
    for ( int i = 0; i < m_iBombCount; i++ )
    {
        pCellIndex = pPossibleIndex[ rand() % ( iBoardSize - iRemovedCount ) ];
        m_pBoard[ pCellIndex[0] ][ pCellIndex[1] ].isMine = true;
        
        GetLimits( limits, pCellIndex[0], pCellIndex[1] );
        for ( int iRowIndex = limits.iRowStart; iRowIndex <= limits.iRowEnd; iRowIndex++ )
            for ( int iColumnIndex = limits.iColumnStart; iColumnIndex <= limits.iColumnEnd; iColumnIndex++ )
                m_pBoard[ iRowIndex ][ iColumnIndex ].value++;
        
        pCellIndex[0] = pPossibleIndex[ iBoardSize - iRemovedCount - 1 ][0];
        pCellIndex[1] = pPossibleIndex[ iBoardSize - iRemovedCount - 1 ][1];
        iRemovedCount++;
    }
    
    for ( int i = 0; i < iBoardSize - iRemovedCount; i++ )
        delete[] pPossibleIndex[i];
    delete[] pPossibleIndex;
}

void Game::RevealCell( const int iRowIndex, const int iColumnIndex )
{
    int iDistanceFromStart = 0;
    RevealCell( iRowIndex, iColumnIndex, iDistanceFromStart );
}

void Game::RevealCell( const int iRowIndex, const int iColumnIndex, int& iDistanceFromStart )
{
    Cell& rCell = m_pBoard[ iRowIndex ][ iColumnIndex ];
    
    if ( m_iRevealedCount == 0 ) PlaceBombs();
    else if ( rCell.isFlagged == true ) return;
    
    if ( rCell.isRevealed == false )
    {
        rCell.isRevealed = true;
        m_iRevealedCount++;
        
        if ( rCell.isMine == true ) m_isEndgame = true;
        
        if ( rCell.value != 0 ) return;
    }
    else if ( iDistanceFromStart != 0 ) return;
    
    iDistanceFromStart++;
    
    Limits limits;
    GetLimits( limits, iRowIndex, iColumnIndex );
    for ( int iRowIndex = limits.iRowStart; iRowIndex <= limits.iRowEnd; iRowIndex++ )
        for ( int iColumnIndex = limits.iColumnStart; iColumnIndex <= limits.iColumnEnd; iColumnIndex++ )
            RevealCell( iRowIndex, iColumnIndex, iDistanceFromStart );
}

void Game::PrintBoard( const bool bRevealBombs = false ) const
{
    system( "cls" );
    bool isSelected;
    for ( int iRowIndex = 0; iRowIndex < m_pBoardSize[0]; iRowIndex++ )
    {
        for ( int iColumnIndex = 0; iColumnIndex < m_pBoardSize[1]; iColumnIndex++ )
        {
            Cell& rCell = m_pBoard[ iRowIndex ][ iColumnIndex ];
            isSelected = iRowIndex == m_pSelectedCell[0] && iColumnIndex == m_pSelectedCell[1];
            
            if ( rCell.isRevealed == false )
            {
                if ( isSelected == true )
                {
                    cout << COLOR_SELECTED << "   " << COLOR_RESET;
                    continue;
                }
                
                if ( bRevealBombs == true && rCell.isMine == true )
                {
                    cout << COLOR_MINE << "   " << COLOR_RESET;
                    continue;
                }
                
                if ( rCell.isFlagged == true )
                {
                    cout << COLOR_FLAG << "   " << COLOR_RESET;
                    continue;
                }
                cout << COLOR_HIDDEN << "   " << COLOR_RESET;
                continue;
            }
            
            if ( rCell.isMine == true )
            {
                cout << COLOR_MINE << "   " << COLOR_RESET;
                continue;
            }
            
            if ( isSelected == true )
            {
                cout << COLOR_SELECTED;
            }
            
            if ( rCell.value == 0 )
            {
                cout << " - " << COLOR_RESET;
                continue;
            }
            
            cout << " " << rCell.value << " " << COLOR_RESET;
        }
        cout << endl;
    }
}

void Game::GameLoop()
{
    int iKey; int pKeys[] = { KEY_ENTER, KEY_SPACE, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW, KEY_ZERO };
    while ( m_isEndgame == false && m_iRevealedCount + m_iBombCount != m_pBoardSize[0] * m_pBoardSize[1] )
    {
        PrintBoard();
        
        iKey = GetKeyFromList( pKeys );
        
        if ( iKey == KEY_UP_ARROW ) MoveSelected( 0, -1 );
        else if ( iKey == KEY_LEFT_ARROW ) MoveSelected( 1, -1 );
        else if ( iKey == KEY_RIGHT_ARROW ) MoveSelected( 1, 1 );
        else if ( iKey == KEY_DOWN_ARROW ) MoveSelected( 0, 1 );
        else if ( iKey == KEY_ENTER || iKey == KEY_SPACE ) RevealCell( m_pSelectedCell[0], m_pSelectedCell[1] );
        else if ( iKey == KEY_ZERO ) PlaceFlag();
    }
    PrintBoard( true );
}



int main()
{
    Game();
    while ( true ){};
    return 0;
}