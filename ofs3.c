#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ofs3.h"
#include "utilities.h"

char *FileNameTable[1024] = { 0 };

void Extract(unsigned char *pInputFilePath, unsigned char *pOutputFolderPath, bool pFileListOutputEnabled)
{
    unsigned char InputFilePath[256];
    memcpy(InputFilePath, pInputFilePath, 256);

    unsigned char OutputFolderPath[256];
    memcpy(OutputFolderPath, pOutputFolderPath, 256);

    FILE *pReadFileOSF3;
    FILE *pWriteFileOSF3;

    //Only used if pFileListOutputEnabled is true
    FILE *pWriteFileList;

    if ((pReadFileOSF3 = fopen(InputFilePath,"rb")) == NULL)
    {
        printf("Error, can't open file.\n");
        printf("Important, verify that path doesn't contains spaces.\n");
        exit(1);
    }

    OFS3 ContainerOFS3;

    unsigned char buffer[4];

    for (int i = 0; i <=16; i = i + 4)
    {
        fread(buffer, sizeof(buffer),1,pReadFileOSF3);

        if (i == 0)
        {
            memcpy(ContainerOFS3.FileSignature, buffer, sizeof(buffer));
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

    for (int i = 0; i < FileCountDec; i++)
    {
        // There is a bug with this stuff, check file boundaries
        if (FileNameTableBuffer[0] != 0x0)
        {
            memcpy(FileArray[i].FileName, FileNameTable[i], sizeof(FileArray[i].FileName));
        }
        else
        {
            memcpy(FileArray[i].FileName, MakeIncrementalFileName(InputFilePath, i), sizeof(FileArray[i].FileName));
        }
    }

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

    unsigned char FileName[128];

    chdir(OutputFolderPath);
    //chdir("/home/pmbonneau/Documents/Test/");

    for (int FileIndex = 0; FileIndex < FileCountDec; FileIndex++)
    {
        int FileSize = CharHexArrayToHexInt(FileArray[FileIndex].FileSize);
        int FileStart = CharHexArrayToHexInt(FileArray[FileIndex].FileStart) + 0x10;
        memcpy(FileName, FileArray[FileIndex].FileName, sizeof(FileArray[FileIndex].FileName));

        if ((pWriteFileOSF3 = fopen(FileName,"wb")) == NULL)
        {
            printf("Error, can't open file.\n");
            printf("Important, verify that path doesn't contains spaces.\n");
            exit(1);
        }

        unsigned char* FileData = malloc(FileSize);

        fseek(pReadFileOSF3, FileStart, SEEK_SET);

        fread(FileData, FileSize,1,pReadFileOSF3);

        fwrite(FileData, FileSize,1,pWriteFileOSF3);

        if (pFileListOutputEnabled == true)
        {
            if ((pWriteFileList = fopen("FileList.txt","a")) == NULL)
            {
                printf("Error, can't open file.\n");
                exit(1);
            }

            int ReturnCode = fputs(FileName, pWriteFileList);
            ReturnCode = fputs("\n", pWriteFileList);

            if (ReturnCode == EOF)
            {
                printf("Error, can't write to file.\n");
            }
        }
    }

    if (pFileListOutputEnabled == true)
    {
        fclose(pWriteFileList);
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
