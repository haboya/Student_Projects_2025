#include "vehicle_detection_config.h"

#define DISP_MSG_LEN 16

char disp_line1_msg[DISP_MSG_LEN+1];
char disp_line2_msg[DISP_MSG_LEN+1];

bool Disp_Init( void )
{
    return true;
}

bool Disp_Update( void )
{
    return true;
}

bool Disp_SetMessage( const char *msg, uint8_t line )
{
    if(
        (strlen( msg ) > DISP_MSG_LEN) ||
        ( msg == NULL ) ||
        ( line < 1 ) ||
        ( line > 2 )
    )
    {
        return false;
    }

    if( line == 1 )
    {
        memset( disp_line1_msg, 0, strlen( disp_line1_msg ) );
        sprintf( disp_line1_msg, "%s", msg );
    }
    else if( line == 2 )
    {
        memset( disp_line2_msg, 0, strlen( disp_line2_msg ) );
        sprintf( disp_line2_msg, "%s", msg );
    }
    else
    {
        return false;
    }

    return true;
}