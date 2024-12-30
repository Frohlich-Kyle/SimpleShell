

#define mSize 1000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int parseInput(char*[]);
int changeDirectories(char*, int);
int executeCommand(char*[], int);

int main()
{
    char command[mSize] = "plhldr";
    char buffer[mSize];
    char* arrayStrings[mSize];
    int numCommands, error;


    while(strcmp(command,"exit") != 0)
    {   
        char* curDirec = getcwd(buffer, sizeof(buffer));
        printf("kfrohlich:%s$ ", curDirec);

        numCommands = parseInput(arrayStrings);

        if(numCommands >= 1)
        {
            strcpy(command, arrayStrings[0]);
        }


        if(strcmp(command, "exit") != 0 && numCommands >= 1)
        {

            if(strcmp(command, "cd") == 0)
            {
                error = changeDirectories(arrayStrings[numCommands - 1], numCommands);
                if(error == -1)
                {
                    printf("Error when changing directory, task not completed\n");
                }
            }
            else
            {
                error = executeCommand(arrayStrings, numCommands);
                if(error == -1)
                {
                    printf("Error when attempting to execute command, task not completed\n");
                }
            }
        }
        //loop ends here
    }
    
    return 0;
}

int parseInput(char* tokenCollec[])
{
    char string[100];
    const char delim[2] = " ", delim2[2] = "\n";
    char* token;
    int index = 0, numspaces = 0, numCommands;
    int exit = 0;
    numCommands = 0;

    fgets(string, sizeof(string), stdin);

    if((strcmp(string,"exit\n")) == 0)
    {
        token = strtok(string, delim2);
        //strcpy(tokenCollec[0],token);
        tokenCollec[0] = token;
        exit = 1;
        numCommands = 1;
    }
    else if((strcmp(string,"\n")) == 0)
    {
        numCommands = 0;
        exit = 1;
    }

    if(exit == 0)
    {
        int size = strlen(string);
        for(int i = 0; i < size; i++)
        {
            if(string[i] == ' ')
            {
                numspaces++;
            }
        }

        if(numspaces == 0)
        {
            token = strtok(string, delim2);
            //strcpy(tokenCollec[index], token);
            tokenCollec[index] = token;
            index++;
        }
        else
        {
            token = strtok(string, delim);
            //strcpy(tokenCollec[index], token);
            tokenCollec[index] = token;
            index++;
        }

        while(index <= numspaces)
        {
            if(index == numspaces)
            {
                token = strtok(NULL, delim2);
                //printf("%s\n", token);
                //token[strlen(token)] = '\0';
                tokenCollec[index] = token;
                index++;
            }
            else
            {
                token = strtok(NULL, delim);
                //printf("%s\n", token);
                //token[strlen(token)] = '\0';
                tokenCollec[index] = token;
                index++;
            }
        }
        numCommands = index;
        tokenCollec[numCommands] = NULL;
    }

    return numCommands;
}

int changeDirectories(char* commands, int numCommands)
{
    char* path;
    int error;
    char remake[mSize];

    if(numCommands != 2)
    {
        printf("Path Not Formatted Correctly!\n");
        return -1;
    }
    else
    {
        //check if the ./ is on the command or not
        if(commands[0] != '.' && commands[1] != '/')
        {
            remake[0] = '.';
            remake[1] = '/';
            int size = strlen(commands);
            for(int index = 0; index < size; index++)
            {
                remake[index + 2] = commands[index];
            }
            path = remake;
        }
        else
        {
            error = chdir(commands);
        }
        error = chdir(path);
        return error;
    }
}

int executeCommand(char* commands[], int numCommands)
{
    int error = 0;

    char* args[mSize];
    for(int index = 0; index < numCommands; index++)
    {
        args[index] = commands[index];
    }
    args[numCommands] = NULL;

    pid_t newFork = fork();
    pid_t newWait = wait(NULL);

    if(newFork == -1)
    {
        error = -1;
        printf("fork Failed: %s", strerror(error));
    }
    else if(newFork == 0)
    {
        error = execvp(commands[0], args);
        if(error == -1)
        {
            error = -1;
            printf("exec Failed: %s", strerror(error));
        }
    }
    else
    {
        if(newWait == -1)
        {
            error = -1;
            strerror(error);
            printf("Child finished with error status: %d\n", error);
            error = -1;
        }
    }
    return error;
}