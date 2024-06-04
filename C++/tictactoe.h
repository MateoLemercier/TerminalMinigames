#pragma once

namespace TicTacToe
{
    
    #define KEY_ENTER 13
    
    #define KEY_UP_ARROW 72
    #define KEY_LEFT_ARROW 75
    #define KEY_RIGHT_ARROW 77
    #define KEY_DOWN_ARROW 80
    
    #define PLAYER_1 'X'
    #define PLAYER_2 'O'
    
    typedef struct Cell
    {
        char cCharacter = '-';
    } Cell;
    
    class Game
    {
    
    public:
        Game( const int iRowCount, const int iColumnCount, const int iWinStreak );
        ~Game();
    
    protected:
        void MoveSelected( const int iAxis, const int iDistance );
        void PrintBoard() const;
        void GameLoop();
    
    protected:
        int m_pBoardSize[2];
        int m_iWinStreak;
        Cell** m_pBoard;
        int m_pSelectedCell[2];
        int m_iPlayer;
        bool m_isEndgame;
        
    };
    
}