// Preprocessor directives that include header files into our program
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Dynamic memory allocation
#include <sys/types.h> // System calls for file input/output and file status information
#include <sys/stat.h>
#include <unistd.h> // POSIX API for Unix-like operating systems

#define MAX 1024

char *exitSmallString = "exit\n";
char *exitBigString = "Exit\n";
char *dirString = "dir\n";
char *cdString = "cd";
char *historyString = "history\n";
int mdFunction(int argc, char *argv[]);
char *ipconfigString = "ipconfig\n";
char *pingString = "ping";
char *psString = "ps\n";

char commandHistory[MAX][MAX];
int commandCount = 0;

int main(int argc, char *argv[])
{
    // clear();
    printf("************************** Running Power X Shell **************************\n\nWritten in C\n");

    // Use `while(1)` to keep on asking the user over and over again for input
    // The only way out is if the program exits
    // And the user will ask for that using "Exit"

    while (1)
    {

        // ------------------- Variables used to get input from the user ----------------------------------------

        char line[MAX]; // Character array to store input

        // ------------------- Variables used for tokenizing a command into words --------------------------------

        char *savedTokens[256] = {NULL}; // The actual derived tokens
        int nTokens = 0;                 // To store the number of tokens

        printf("\nPower X Shell> ");      // Display the command prompt
        fgets(line, sizeof(line), stdin); // Getting input from user

        // Store the entered command in the history array
        strcpy(commandHistory[commandCount], line);
        commandCount++;

        // Deep copy of input string, stored in character pointer
        char *cinput = malloc(strlen(line) + 1);
        strcpy(cinput, line);

        // Input string is tokenized to words
        char *tokens = strtok(cinput, " ");

        while (tokens != NULL)
        {
            // Using `strdup()` here since we want to deep copy the C string into `savedTokens` and not just its pointer

            savedTokens[nTokens] = strdup(tokens);
            nTokens++;
            tokens = strtok(NULL, " ");
        }
        // Done tokenizing the command into arguments

        // Check if the first token is "history"
        if (strcmp(historyString, savedTokens[0]) == 0)
        {
            printf("\n----- Command History -----\n");
            for (int i = 0; i < commandCount; i++)
            {
                printf("%d. %s", i + 1, commandHistory[i]);
            }
            printf("----------------------------\n");
            continue;
        }

        if (strcmp(exitSmallString, savedTokens[0]) == 0 || strcmp(exitBigString, savedTokens[0]) == 0)
        {
            printf("Exiting now! Remember to always keep your shell up!\n\nShell shutting down ...\n");
            return 0;
        }

        if (strcmp(pingString, savedTokens[0]) == 0)
        {
            if (nTokens < 2)
            {
                printf("Error: Insufficient arguments for ping command.\n");
            }
            else
            {
                char *hostname = savedTokens[1];
                char command[256];
                sprintf(command, "ping -c 5 %s", hostname); // Construct the ping command with 5 packets
                system(command);
            }
            continue;
        }

        // Check if the first token is "dir"
        if (strcmp(dirString, savedTokens[0]) == 0)
        {
            char cwd[1024];                                 // Buffer to store the current working directory
            getcwd(cwd, sizeof(cwd));                       // Get the current working directory
            printf("Current working directory: %s\n", cwd); // Display the current working directory
            continue;                                       // Continue with the next iteration of the loop
        }

        // Check if the first token is "cd"
        if (strcmp(cdString, savedTokens[0]) == 0)
        {
            if (chdir(savedTokens[1]) == -1)
            {
                printf("Error: Directory does not exist or cannot be accessed.\n");
            }
            else
            {
                char cwd[1024];
                getcwd(cwd, sizeof(cwd));
                printf("Current working directory: %s\n", cwd);
            }
            continue;
        }

        if (strcmp(ipconfigString, savedTokens[0]) == 0)
        {
            // Execute the ipconfig command using system()
            system("ipconfig");
            continue; // Continue with the next iteration of the loop
        }

        // Check if the first token is "copy"
        if (strcmp("copy", savedTokens[0]) == 0)
        {
            // Check if the user has provided enough arguments
            if (nTokens < 3)
            {
                printf("Error: Please provide at least two file names to copy.\n");
            }
            else
            {
                // Get the source file name(s) and the destination directory
                char *sourceFiles[nTokens - 2];
                char *destDir = savedTokens[nTokens - 1];
                for (int i = 1; i < nTokens - 1; i++)
                {
                    sourceFiles[i - 1] = savedTokens[i];
                }
                // Copy each file to the destination directory
                for (int i = 0; i < nTokens - 2; i++)
                {
                    char *sourcePath = malloc(strlen(sourceFiles[i]) + 1 + strlen(destDir) + 1);
                    sprintf(sourcePath, "%s %s/%s", sourceFiles[i], destDir, sourceFiles[i]);
                    int status = system(sourcePath);
                    if (status == 0)
                    {
                        printf("%s has been copied successfully.\n", sourceFiles[i]);
                    }
                    else
                    {
                        perror(sourcePath);
                    }
                    free(sourcePath);
                }
            }
            continue;
        }

        // Check if the first token is "rm"
        if (strcmp("rm", savedTokens[0]) == 0)
        {
            // Check if the user has provided the correct number of arguments
            if (nTokens < 2)
            {
                printf("Error: Please provide a file name to remove.\n");
            }
            else
            {
                // Remove the specified file
                if (remove(savedTokens[1]) != 0)
                {
                    printf("Error: Unable to remove file.\n");
                }
                else
                {
                    printf("File removed successfully.\n");
                }
            }
            continue;
        }

        for (int i = 0; i < nTokens; i++)
        {
            printf("SavedTokens[%d] = %s\n", i, savedTokens[i]);
        }
    }

    return 0;
}

int mdFunction(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s directory_name\n", argv[0]);
        return 1;
    }

    char *dir_name = argv[1];

    // Check if directory already exists
    struct stat st;
    if (stat(dir_name, &st) == 0)
    {
        fprintf(stderr, "Error: Directory '%s' already exists.\n", dir_name);
        return 1;
    }

    // Create the new directory
    if (mkdir(dir_name, 0777) == -1)
    {
        fprintf(stderr, "Error: Failed to create directory '%s'.\n", dir_name);
        return 1;
    }

    printf("Successfully created directory '%s'\n", dir_name);

    return 0;
}