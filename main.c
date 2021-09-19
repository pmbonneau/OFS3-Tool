#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ofs3.h"
#include "utilities.h"

int main()
{
    printf(" #######  ########  ######   #######        ########  #######   #######  ##\n");
    printf("##     ## ##       ##    ## ##     ##          ##    ##     ## ##     ## ##\n");
    printf("##     ## ##       ##              ##          ##    ##     ## ##     ## ##\n");
    printf("##     ## ######    ######   #######           ##    ##     ## ##     ## ##\n");
    printf("##     ## ##             ##        ##          ##    ##     ## ##     ## ##\n");
    printf("##     ## ##       ##    ## ##     ##          ##    ##     ## ##     ## ##\n");
    printf(" #######  ##        ######   #######           ##     #######   #######  ########\n");
    printf("\n");
    printf("Welcome to OFS3 tool!\n");
    printf("\n");
    printf("Options list :\n");
    printf("1. Extract an OFS3 file\n");
    printf("2. Extract an OFS3 file recursively\n");
    printf("3. Extract all OFS3 files from a folder\n");
    printf("4. Exit\n");
    printf("\n");

    int OptionChoice;
    scanf("%d", &OptionChoice);

    printf("Choice = %d", OptionChoice);
    printf("\n");

    if (OptionChoice == 1)
    {
        printf("Selected : 1\n");
        printf("Enter the path of the OFS3 file to extract.\n");

        char InputFilePath[256];
        scanf("%s",InputFilePath);
        printf("%s", InputFilePath);

        printf("\n");
        printf("Enter the output folder path.\n");

        char OutputFolderPath[256];
        scanf("%s",OutputFolderPath);
        printf("%s", OutputFolderPath);

        Extract(InputFilePath, OutputFolderPath, false);
    }

    if (OptionChoice == 2)
    {
        printf("Selected : 2\n");
        printf("Enter the path of a folder that contains OFS3 files to extract.\n");

        char InputFolderPath[256];
        scanf("%s",InputFolderPath);
        printf("%s", InputFolderPath);

        printf("\n");
        printf("Enter the output folder path.\n");

        char OutputFolderPath[256];
        scanf("%s",OutputFolderPath);
        printf("%s", OutputFolderPath);

        DIR *SelectedDirectory;
        struct dirent *Directory;
        SelectedDirectory = opendir(InputFolderPath);
        char CurrentFileName[256];
        char CurrentFilePath[256];

        if (SelectedDirectory)
        {
            while ((Directory = readdir(SelectedDirectory)) != NULL)
            {
                memcpy(CurrentFileName, Directory->d_name, 256);
                //if (CurrentFileName[0] != '.')
                if (Directory->d_type==DT_REG)
                {
                    // Resets the CurrentFilePath array.
                    CurrentFilePath[0] = '\0';

                    // Read directory only returns filename, so we append the full path to it.
                    strcat(CurrentFilePath, InputFolderPath);
                    strcat(CurrentFilePath, "/");
                    strcat(CurrentFilePath,Directory->d_name);
                    printf("%s\n", CurrentFilePath);
                    Extract(CurrentFilePath, OutputFolderPath, true);
                }
            }
            closedir(SelectedDirectory);
        }
    }

    return 0;
}
