#pragma once

int Find( const int iInt, const int* const pList );
int GetKeyFromList( const int* const pList );

namespace Minesweeper
{
    
    #define KEY_SPACE 32
    #define KEY_ENTER 13
    
    #define KEY_ZERO 48
    
    #define KEY_UP_ARROW 72
    #define KEY_LEFT_ARROW 75
    #define KEY_RIGHT_ARROW 77
    #define KEY_DOWN_ARROW 80
    
    
    #define COLOR_RESET "\033[0m"
    #define COLOR_HIDDEN "\033[0;42m"
    #define COLOR_FLAG "\033[0;43m"
    #define COLOR_MINE "\033[0;41m"
    #define COLOR_SELECTED "\033[0;44m"
    
    
    #define VERTICAL_LINE (char)179
    
    
    
    typedef struct Cell
    {
        int value = 0;
        bool isRevealed = false;
        bool isFlagged = false;
        bool isMine = false;
    } Cell;
    
    
    typedef struct Limits
    {
        int iRowStart;
        int iRowEnd;
        int iColumnStart;
        int iColumnEnd;
    } Limits;
    
    
    
    class Game
    {
    
    public:
        Game( const int iRowCount, const int iColumnCount, const int iBombCount );
        ~Game();
    
    protected:
        void MoveSelected( const int iAxis, const int iDistance );
        void PlaceFlag();
        void GetLimits( Limits& limits, const int iRowIndex, const int iColumnIndex );
        void PlaceBombs();
        void RevealCell( const int iRowIndex, const int iColumnIndex );
        void RevealCell( const int iRowIndex, const int iColumnIndex, int& iDistanceFromStart );
        void PrintBoard( const bool bRevealBombs ) const;
        void GameLoop();
    
    protected:
        int m_pBoardSize[2];
        int m_iBombCount;
        Cell** m_pBoard;
        int m_pSelectedCell[2];
        int m_iRevealedCount;
        int m_iFlaggedCount;
        bool m_isEndgame;
    
    };
    
}