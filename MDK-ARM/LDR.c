#include "LDR.h"

int checkDay(uint16_t data)
{
    if(data > 1000)	
    {
        return 1;
    }
    else return 0;
}
