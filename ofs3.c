#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ofs3.h"
#include "utilities.h"

char *FileNameTable[1024] = { 0 };

void Extract(unsigned char *FilePath)
{
    FILE *pReadFileOSF3;
    FILE *pWriteFileOSF3;

    if ((pReadFileOSF3 = fopen("/home/pmbonneau/Documents/OFS3 Samples/asuna.ofs3","rb")) == NULL)
    {
        printf("error!\n");
        exit(1);
    }

    OFS3 ContainerOFS3;

    unsigned char buffer[4];

    for (int i = 0; i <=16; i = i + 4)
    {
        //fseek(pReadFileOSF3, i, SEEK_CUR);
        fread(buffer, sizeof(buffer),1,pReadFileOSF3);

        if (i == 0)
        {
            memcpy(ContainerOFS3.FileSignature, buffer, sizeof(buffer));
            //sscanf(buffer, "%x", &testint);
        }

        if (i == 4)
        {
            memcpy(ContainerOFS3.HeaderSize, buffer, sizeof(buffer));
        }

        if (i == 8)
        {
            memcpy(ContainerOFS3.Unknown1, buffer, sizeof(buffer));
        }

        if (i == 12)
        {
            memcpy(ContainerOFS3.FileSize, buffer, sizeof(buffer));
        }

        if (i == 16)
        {
            memcpy(ContainerOFS3.FileCount, buffer, sizeof(buffer));
        }

        for (int j = 0; j < sizeof(buffer); j++)
        {
            printf("%02x\n", buffer[j]);
        }
    }


    // Extraction algorithm thinkering
    // Example of OFS3 header
    // [4F 46 53 33] [10 00 00 00] [00 00 40 00] [F0 9B 7E 00]
    // [04 00 00 00] [30 00 00 00] [C0 9A 7E 00] [F0 9A 7E 00]
    // [1C 00 00 00] [30 9B 7E 00] [28 00 00 00] [70 9B 7E 00]
    // [80 00 00 00] [00 00 00 00] [00 00 00 00] [00 00 00 00]

    // 1 - We read the first 20 bytes to get the container info. Already done above
    // Signature, header size, unknown1, filesize and filecount

    //1.5 Get file names
    // We know that file name table comes right after ContainerOFS3.FileSize, + 0x10 bytes.
    int ContainerSize = CharHexArrayToHexInt(ContainerOFS3.FileSize) + 0x10;

    // We will need those too.
    int FileCountHex = CharHexArrayToHexInt(ContainerOFS3.FileCount);
    int FileCountDec = HexIntToDecInt(FileCountHex);

    // For file names table
    fseek(pReadFileOSF3, ContainerSize, SEEK_SET);

    // Let's create a file name buffer that is big enough to contain all file names. Soon, that buffer will be dynamically allocated
    // depending of FileCountDec =)
    unsigned char FileNameTableBuffer[1024];
    //unsigned char** FileNameTable;
    fread(FileNameTableBuffer, sizeof(FileNameTableBuffer),1,pReadFileOSF3);

    // 2 - We declare the stuff we need for files info like this
    DataFile FileArray[FileCountDec]; // Replace 4 with FileCount, one file is one DataFile object.

    unsigned char** FileNameTable;
    //const char* FileNames[8];
    FileNameTable = GetFileNameTable(FileNameTableBuffer, 2);

    // Max. filename size is 128
    unsigned char FileNames[128];
    for (int i = 0; i < FileCountDec; i++)
    {
        memcpy(FileArray[i].FileName, FileNameTable[i], sizeof(FileNames));
    }

    // We will have to build a loop that will gather the required file info.

    // At this point, pReadFileOFS3 should be at offset 0x14 (right after the OFS3 header + FileCount
    fseek(pReadFileOSF3, 0x14, SEEK_SET);
    for (int FileIndex = 0; FileIndex < FileCountDec; FileIndex++)
    {
        // Get the file postion (file start)
        fread(buffer, sizeof(buffer),1,pReadFileOSF3);
        memcpy(FileArray[FileIndex].FileStart, buffer, sizeof(buffer));

        // Get the file size
        fread(buffer, sizeof(buffer),1,pReadFileOSF3);
        memcpy(FileArray[FileIndex].FileSize, buffer, sizeof(buffer));

    }

    chdir("/home/pmbonneau/Documents/OFS3 Samples/");

    for (int FileIndex = 0; FileIndex < FileCountDec; FileIndex++)
    {
        int FileSize = CharHexArrayToHexInt(FileArray[FileIndex].FileSize);
        int FileStart = CharHexArrayToHexInt(FileArray[FileIndex].FileStart) + 0x10;
        unsigned char* FileName[128];
        memcpy(FileName, FileArray[FileIndex].FileName, sizeof(FileArray[FileIndex].FileName));

        if ((pWriteFileOSF3 = fopen(FileName,"wb")) == NULL)
        {
            printf("error!\n");
            exit(1);
        }


        unsigned char* FileData = malloc(FileSize);

        fseek(pReadFileOSF3, FileStart, SEEK_SET);

        fread(FileData, FileSize,1,pReadFileOSF3);

        fwrite(FileData, FileSize,1,pWriteFileOSF3);
    }

    fclose(pWriteFileOSF3);
    fclose(pReadFileOSF3);

    return;
}

// Hard stuff
// Example of nametable :
// [63 68 30 31 5F 30 30 30 5F 2E 74 67 61 2E 70 68
// 79 72 65] 00 [63 68 30 31 5F 30 30 30 5 F 2E 62 75
// 76] 00 00 00 00 00 00 00

// [ch01_000_.tga.phyre]
// [ch01_000_.buv]
unsigned char** GetFileNameTable(unsigned char *pFileNameTableBuffer, int FileCount)
{
    unsigned char FileNameTableBuffer[1024];
    unsigned char** FileNameTable;

    memcpy(FileNameTableBuffer, pFileNameTableBuffer, sizeof(FileNameTableBuffer));

    char *cursor = FileNameTableBuffer;

    FileNameTable = malloc(FileCount * sizeof(char*));

    for (int i = 0; i < FileCount; i++)
    {
        int FileNameLength = strlen(cursor);
        FileNameTable[i] = malloc((FileNameLength + 1) * sizeof(char*));
        strcpy(FileNameTable[i], cursor);
        cursor += FileNameLength + 1;
    }
    return FileNameTable;
}
