#include <stdio.h>
#include <stdlib.h>
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

        if ((pReadFileOSF3 = fopen("/home/pmbonneau/Documents/OSF3 Samples/sample.ofs3","rb")) == NULL)
        {
            printf("error!\n");
            exit(1);
        }

        unsigned char buffer[16];

        fread(buffer, sizeof(buffer),1,pReadFileOSF3);

        printf("%s", buffer);


        OFS3 OFS3File;

        if ((pWriteFileOSF3 = fopen("/home/pmbonneau/Documents/OSF3 Samples/test.bin","wb")) == NULL)
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
