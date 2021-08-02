#include "utilities.h"

int CharHexArrayToHexInt(unsigned char *HexArray)
{
    int HexInteger = HexArray[0] | ((int)HexArray[1] << 8 | (int)HexArray[2] << 16 | (int)HexArray[3] << 24 );
    return HexInteger;
}

int HexIntToDecInt(int HexInt)
{
    int DecInt;
    unsigned char buffer[16];
    sprintf(buffer, "%d", HexInt);
    sscanf(buffer, "%d", &DecInt);

    return DecInt;
}
