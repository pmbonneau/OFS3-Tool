#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ofs3.h"
#include "utilities.h"

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
        printf("Selected : 1\n");
        printf("Enter the path of the OSF3 file to extract.\n");

        char InputFilePath[256];
        scanf("%s",InputFilePath);
        printf("%s", InputFilePath);

        printf("\n");
        printf("Enter the output folder path.\n");

        char OutputFolderPath[256];
        scanf("%s",OutputFolderPath);
        printf("%s", OutputFolderPath);

        Extract(InputFilePath, OutputFolderPath);
    }

    return 0;
}
