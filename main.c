#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ofs3.h"

int main()
{
    printf("Welcome to OSF3 tool!\n");
    printf("\n");
    printf("Options list :\n");
    printf("1. Extract an OSF3 file\n");
    printf("2. Extract all OSF3 files from a folder\n");
    printf("3. Exit\n");
    printf("\n");

    int OptionChoice;
    scanf("%d", &OptionChoice);

    printf("Choice = %d", OptionChoice);
    printf("\n");

    if (OptionChoice == 1)
    {
        printf("1\n");
        printf("Enter the path of the OSF3 file to extract.\n");

        char FilePath[256];
        scanf("%s",FilePath);

        printf("%s", FilePath);

        FILE *pReadFileOSF3;
        FILE *pWriteFileOSF3;

        if ((pReadFileOSF3 = fopen("/home/pmbonneau/Documents/OFS3 Samples/char.ofs3","rb")) == NULL)
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

        // 2 - We declare the stuff we need for files info like this
        DataFile FileArray[4]; // Replace 4 with FileCount, one file is one DataFile object.
        // We will have to build a loop that will gather the required file info.

        memcpy(FileArray[1].FileStart, buffer, sizeof(buffer));
        memcpy(FileArray[1].FileSize, buffer, sizeof(buffer));




        if ((pWriteFileOSF3 = fopen("/home/pmbonneau/Documents/OFS3 Samples/test.bin","wb")) == NULL)
        {
            printf("error!\n");
            exit(1);
        }

        fwrite(buffer, sizeof(buffer),1,pWriteFileOSF3);

        fclose(pWriteFileOSF3);
        fclose(pReadFileOSF3);
    }

    return 0;
}
