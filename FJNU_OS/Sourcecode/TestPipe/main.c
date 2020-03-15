#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define BUF_LENGTH 1024

int main()
{
    int fd[2];

    if (pipe(fd) < 0) {
        perror("pipe failed.\n");
        return -1;
    }

    pid_t pid = fork();
    if (pid > 0) {
        close(fd[0]);
        printf("Server: Input message body. \n");
        fflush(stdout);
        char buf[BUF_LENGTH];
        memset(buf, '\0', sizeof(char)*BUF_LENGTH);
        ssize_t s = read(0, buf, sizeof(buf));
        if (s>0) {
            buf[s-1] = '\0';
        }
        else {
            strcpy(buf, "(empty)");
        }
        write(fd[1], buf, strlen(buf));
        printf("Server: Sending message. [OK] \n");

        wait(NULL);
        printf("Parent process exits. \n");
    }
    else if (pid == 0) {
        close(fd[1]);
        char buf[BUF_LENGTH];
        memset(buf, '\0', sizeof(char)*BUF_LENGTH);
        read(fd[0], buf, BUF_LENGTH);
        if (strlen(buf) > 0) {
            printf("Client: Server says %s. \n", buf);
        }
        else {
            printf("Client: Server says nothing. \n");
        }
        printf("Child process exits.\n");
    }

    return 0;
}
