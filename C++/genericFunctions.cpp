#include "genericFunctions.h"

int CharInString( const char cChar, const char* const sString )
{
    for ( int index = 0; sString[index] != '\0'; index++ )
        if ( sString[index] == cChar )
            return index;
    return -1;
}