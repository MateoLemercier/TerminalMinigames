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



Game::Game( const int iRowCount = 14, const int iColumnCount = 18, const int iMineCount = 40 )
{
    m_pBoardSize[0] = iRowCount;
    m_pBoardSize[1] = iColumnCount;
    m_iMineCount = iMineCount;
    
    m_pBoard = new Cell*[ m_pBoardSize[0] ];
    for ( int iCellIndex = 0; iCellIndex < m_pBoardSize[0]; iCellIndex++ )
        m_pBoard[ iCellIndex ] = new Cell[ m_pBoardSize[1] ];
    
    m_pSelectedCell[0] = ( m_pBoardSize[0] - 1 ) / 2;
    m_pSelectedCell[1] = ( m_pBoardSize[1] - 1 ) / 2;
    
    m_pMinesList = new int*[ m_iMineCount ];
    for ( int iMineIndex = 0; iMineIndex < m_iMineCount; iMineIndex++ )
        m_pMinesList[ iMineIndex ] = new int[2];
    
    m_pFlagsList = new int*[ m_iMineCount ];
    for ( int iFlagIndex = 0; iFlagIndex < m_iMineCount; iFlagIndex++ )
        m_pFlagsList[ iFlagIndex ] = new int[2];
    
    m_iRevealedCount = 0;
    m_iFlagCount = 0;
    
    m_isEndgame = false;
    srand( time( NULL ) );
    
    GameLoop();
}

Game::~Game()
{
    for ( int iCellIndex = 0; iCellIndex < m_pBoardSize[0]; iCellIndex++ ) delete[] m_pBoard[ iCellIndex ];
    delete[] m_pBoard;
    for ( int iMineIndex = 0; iMineIndex < m_iMineCount; iMineIndex++ ) delete[] m_pMinesList[ iMineIndex ];
    delete[] m_pMinesList;
    for ( int iFlagIndex = 0; iFlagIndex < m_iMineCount; iFlagIndex++ ) delete[] m_pFlagsList[ iFlagIndex ];
    delete[] m_pFlagsList;
}

void Game::MoveSelected( const int iAxis, const int iDistance )
{
    if ( 0 <= m_pSelectedCell[ iAxis ] + iDistance && m_pSelectedCell[ iAxis ] + iDistance < m_pBoardSize[ iAxis ] )
        m_pSelectedCell[ iAxis ] += iDistance;
}

void Game::ClearFlags()
{
    for ( int iFlagIndex = 0; iFlagIndex < m_iFlagCount; iFlagIndex++ )
    {
        Cell& rCell = m_pBoard[ m_pFlagsList[ iFlagIndex ][0] ][ m_pFlagsList[ iFlagIndex ][1] ];
        rCell.isFlagged = false;
        rCell.iColor = COLOR_BASE;
    }
    m_iFlagCount = 0;
}

void Game::PlaceFlag()
{
    Cell& rSelectedCell = m_pBoard[ m_pSelectedCell[0] ][ m_pSelectedCell[1] ];
    if ( rSelectedCell.isRevealed == true ) return;
    
    bool isFlagged = rSelectedCell.isFlagged;
    
    if ( isFlagged == true )
    {
        rSelectedCell.isFlagged = false;
        rSelectedCell.iColor = COLOR_BASE;
        m_iFlagCount--;
        return;
    }
    
    if ( m_iFlagCount < m_iMineCount )
    {
        rSelectedCell.isFlagged = true;
        rSelectedCell.iColor = COLOR_FLAG;
        m_pFlagsList[ m_iFlagCount ][0] = m_pSelectedCell[0];
        m_pFlagsList[ m_iFlagCount ][1] = m_pSelectedCell[1];
        m_iFlagCount++;
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

void Game::PlaceMines()
{
    int iBoardSize = m_pBoardSize[0] * m_pBoardSize[1];
    int** pPossibleIndex = new int*[ iBoardSize ];
    int iRemovedCount = 0;
    int iRowIndex; int iColumnIndex;
    for ( int iCellIndex = 0; iCellIndex < iBoardSize; iCellIndex++ )
    {
        iRowIndex = iCellIndex / m_pBoardSize[1];
        iColumnIndex = iCellIndex % m_pBoardSize[1];
        if ( m_pSelectedCell[0] - 1 <= iRowIndex && iRowIndex <= m_pSelectedCell[0] + 1 && m_pSelectedCell[1] - 1 <= iColumnIndex && iColumnIndex <= m_pSelectedCell[1] + 1 )
        {
            iRemovedCount++;
            continue;
        }
        pPossibleIndex[ iCellIndex - iRemovedCount ] = new int[2];
        pPossibleIndex[ iCellIndex - iRemovedCount ][0] = iRowIndex;
        pPossibleIndex[ iCellIndex - iRemovedCount ][1] = iColumnIndex;
    }
    
    int* pCellIndex;
    Limits limits;
    for ( int iMineIndex = 0; iMineIndex < m_iMineCount; iMineIndex++ )
    {
        pCellIndex = pPossibleIndex[ rand() % ( iBoardSize - iRemovedCount ) ];
        m_pBoard[ pCellIndex[0] ][ pCellIndex[1] ].value += 8;
        
        GetLimits( limits, pCellIndex[0], pCellIndex[1] );
        for ( int iRowIndex = limits.iRowStart; iRowIndex <= limits.iRowEnd; iRowIndex++ )
            for ( int iColumnIndex = limits.iColumnStart; iColumnIndex <= limits.iColumnEnd; iColumnIndex++ )
                m_pBoard[ iRowIndex ][ iColumnIndex ].value++;
        
        m_pMinesList[ iMineIndex ][0] = pCellIndex[0];
        m_pMinesList[ iMineIndex ][1] = pCellIndex[1];
        pCellIndex[0] = pPossibleIndex[ iBoardSize - iRemovedCount - 1 ][0];
        pCellIndex[1] = pPossibleIndex[ iBoardSize - iRemovedCount - 1 ][1];
        iRemovedCount++;
    }
    
    for ( int iCellIndex = 0; iCellIndex < iBoardSize - iRemovedCount; iCellIndex++ )
        delete[] pPossibleIndex[ iCellIndex ];
    delete[] pPossibleIndex;
}

void Game::RevealMines()
{
    for ( int iMineIndex = 0; iMineIndex < m_iMineCount; iMineIndex++ )
        m_pBoard[ m_pMinesList[ iMineIndex ][0] ][ m_pMinesList[ iMineIndex ][1] ].iColor = COLOR_MINE;
}

void Game::RevealCell( const int iRowIndex, const int iColumnIndex )
{
    int iDistanceFromStart = 0;
    RevealCell( iRowIndex, iColumnIndex, iDistanceFromStart );
}

void Game::RevealCell( const int iRowIndex, const int iColumnIndex, int& iDistanceFromStart )
{
    Cell& rCell = m_pBoard[ iRowIndex ][ iColumnIndex ];
    
    if ( m_iRevealedCount == 0 )
    {
        ClearFlags();
        PlaceMines();
    }
    else if ( rCell.isFlagged == true ) return;
    
    if ( rCell.isRevealed == false )
    {
        rCell.isRevealed = true;
        m_iRevealedCount++;
        
        if ( rCell.value >= 9 )
        {
            m_isEndgame = true;
            return;
        }
        
        rCell.iColor = 0;
        
        if ( rCell.value != 0 )
        {
            rCell.cCharacter = '0' + rCell.value;
            return;
        }
        
        rCell.cCharacter = '-';
    }
    else if ( iDistanceFromStart != 0 ) return;
    
    iDistanceFromStart++;
    
    Limits limits;
    GetLimits( limits, iRowIndex, iColumnIndex );
    for ( int iRowIndex = limits.iRowStart; iRowIndex <= limits.iRowEnd; iRowIndex++ )
        for ( int iColumnIndex = limits.iColumnStart; iColumnIndex <= limits.iColumnEnd; iColumnIndex++ )
            RevealCell( iRowIndex, iColumnIndex, iDistanceFromStart );
}

void Game::PrintBoard() const
{
    system( "cls" );
    for ( int iRowIndex = 0; iRowIndex < m_pBoardSize[0]; iRowIndex++ )
    {
        for ( int iColumnIndex = 0; iColumnIndex < m_pBoardSize[1]; iColumnIndex++ )
        {
            Cell& rCell = m_pBoard[ iRowIndex ][ iColumnIndex ];
            cout << "\033[";
            if ( iRowIndex == m_pSelectedCell[0] && iColumnIndex == m_pSelectedCell[1] && rCell.iColor != COLOR_MINE ) cout << COLOR_SELECTED;
            else cout << rCell.iColor;
            cout << "m " << rCell.cCharacter << " " << COLOR_RESET;
        }
        cout << endl;
    }
}

void Game::GameLoop()
{
    int iKey; int pKeys[] = { KEY_ENTER, KEY_SPACE, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW, KEY_ZERO };
    while ( m_isEndgame == false && m_iRevealedCount + m_iMineCount != m_pBoardSize[0] * m_pBoardSize[1] )
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
    RevealMines();
    PrintBoard();
}



int main()
{
    Game();
    while ( true ){};
    return 0;
}