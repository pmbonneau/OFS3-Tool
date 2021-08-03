#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ofs3.h"
#include "utilities.h"

void Extract(unsigned char *FilePath)
{
    FILE *pReadFileOSF3;
    FILE *pWriteFileOSF3;
    //FILE *pWriteFileOSF32;

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
    fread(FileNameTableBuffer, sizeof(FileNameTableBuffer),1,pReadFileOSF3);

    //const char* FileNames[8];
    GetFileNameTable(FileNameTableBuffer);

    //if ((pWriteFileOSF32 = fopen("/home/pmbonneau/Documents/OFS3 Samples/test2.bin","wb")) == NULL)
    //{
      //  printf("error!\n");
       // exit(1);
    //}

    //fwrite(FileNameTableBuffer, 1024,1,pWriteFileOSF32);
    //fclose(pWriteFileOSF32);

    // 2 - We declare the stuff we need for files info like this
    DataFile FileArray[FileCountDec]; // Replace 4 with FileCount, one file is one DataFile object.
    // We will have to build a loop that will gather the required file info.

    // At this point, pReadFileOFS3 should be at offset 0x14
    for (int FileIndex = 0; FileIndex < FileCountDec; FileIndex++)
    {
        // Get the file postion (file start)
        fread(buffer, sizeof(buffer),1,pReadFileOSF3);
        memcpy(FileArray[FileIndex].FileStart, buffer, sizeof(buffer));

        // Get the file size
        fread(buffer, sizeof(buffer),1,pReadFileOSF3);
        memcpy(FileArray[FileIndex].FileSize, buffer, sizeof(buffer));

    }

    if ((pWriteFileOSF3 = fopen("/home/pmbonneau/Documents/OFS3 Samples/test.bin","wb")) == NULL)
    {
       printf("error!\n");
       exit(1);
    }

    for (int FileIndex = 0; FileIndex < 1; FileIndex++)
    {
        int FileSize = CharHexArrayToHexInt(FileArray[FileIndex].FileSize);
        int FileStart = CharHexArrayToHexInt(FileArray[FileIndex].FileStart) + 0x10;

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
void GetFileNameTable(unsigned char *FileNameTableBuffer)
{
    unsigned char FileNames[8][32]; // Name list size
    unsigned char buffer[32]; // Max. filename characters count

    // Initialize
    for (int x = 0; x > 8; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            FileNames[x][y] = NULL;
        }
    }

    int FileNameIndex = 0;
    int j = 0;
    int k = 0;
    for (int i = 0; i < 1024; i++)
    {
        if (FileNameTableBuffer[i] != NULL)
        {
            //FileNames[j][k] = FileNameTableBuffer[i];
            buffer[j] = FileNameTableBuffer[i];
            j++;
        }
        else
        {
            //FileNames[FileNameIndex] = buffer;
            //FileNameIndex++;
            for (int l = 0; l < j; l++)
            {
                FileNames[k][l] = buffer[l];
            }
            j = 0;
            k++;
            //j++;
        }
    }
}