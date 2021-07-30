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
                memcpy(ContainerOFS3.FileNamesTableStart, buffer, sizeof(buffer));
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
