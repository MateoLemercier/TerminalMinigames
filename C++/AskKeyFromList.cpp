#include "AskKeyFromList.h"
#include "genericFunctions.h"
#include <conio.h>

int AskKeyFromList( const int* const pList )
{
    char cUser;
    int iSize = sizeof( pList );
    char* const sList = new char[ iSize ];
    for ( int i = 0; i < iSize; i++ )
        sList[i] = pList[i];
    
    while ( true )
    {
        if ( kbhit() )
        {
            cUser = getch();
            if ( CharInString( cUser, sList ) != -1 )
                delete[] sList;
                return cUser;
        }
    }
}