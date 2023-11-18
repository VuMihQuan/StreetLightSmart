#include "LDR.h"

int checkDay(uint16_t data)
{
    if(data > 500)	
    {
        return 1;
    }
    else return 0;
}
