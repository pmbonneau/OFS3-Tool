#include "utilities.h"
#include <stdio.h>

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

    //for (int i = 0; i <= sizeof(TempFileName); i++)
    //{
     //   FileName[i] = TempFileName[i+1];
    //}

    // Pointer trick to remove leading slash
    FileName = TempFileName + 1;

    return FileName;
}

unsigned char* MakeIncrementalFileName(unsigned char *InputFileName, int Increment)
{
    unsigned char FileName[128];

    // We must malloc() the returned variable
    unsigned char *FileNameAdjusted;
    FileNameAdjusted = malloc(128);

    memcpy(FileName, GetFileName(InputFileName),sizeof(FileName));

    int FileNameLength = 0;
    FileNameLength = strlen(FileName);

    int FileExtPosition = 0;
    for (int j = 0; j < FileNameLength; j++)
    {
        if (FileName[j] == '.')
        {
            FileExtPosition = j;
        }
    }

    for (int h = 0; h < FileExtPosition; h++)
    {
        FileNameAdjusted[h] = FileName[h];
    }

    FileNameAdjusted[FileExtPosition] = '-';

    // Put a 0 in the filename might cause unexpected behaviors, so we start index from 1.
    // 49 is the ASCII number 1, otherwise we get weird characters in file names.
    char FileIndex = Increment + 49;
    FileNameAdjusted[FileExtPosition + 1] = FileIndex;

    for (int k = FileExtPosition + 1; k < FileNameLength + 1; k++)
    {
        FileNameAdjusted[k + 1] = FileName[k - 1];
        if (k == FileNameLength)
        {
            // Be sure that the string is NULL terminated
            FileNameAdjusted[k + 2] = NULL;
        }
    }
    return FileNameAdjusted;
}
