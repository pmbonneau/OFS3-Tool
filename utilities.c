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

int ContainsAscii(unsigned char *pCharArray)
{
    char FirstArrayCharacter;
    char CharArray[1024];
    memcpy(CharArray, pCharArray, 1024);

    FirstArrayCharacter = CharArray[0];

    // 0 = false
    // 1 = true
    int Result = 0;

    if (FirstArrayCharacter >= 97 && FirstArrayCharacter <= 122 || FirstArrayCharacter >= 65 && FirstArrayCharacter <= 90)
    {
        Result = 1;
    }

    return Result;
}

unsigned char* GetFileName(unsigned char *pFilePath)
{
    //unsigned char FilePath[128];

    //memcpy(FilePath, pFilePath, 128);

    unsigned char *TempFileName;

    TempFileName = strrchr(pFilePath, '/');

    unsigned char *FileName;

    FileName = malloc(128);

    for (int i = 0; i <= sizeof(TempFileName); i++)
    {
        FileName[i] = TempFileName[i+1];
    }

    return FileName;
}
