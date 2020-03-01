#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        printf("Fork Failed.\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        execlp("/usr/bin/top", "top", NULL);
    }
    else
    {
        wait(NULL);
        printf("Child Process Completed.\n");
    }
}
