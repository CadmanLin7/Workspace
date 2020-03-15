#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_SIZE 1024

int main()
{
    int ret = fork();
    if (ret == 0) {    // child process reads.
        int fd = 0; int ii = 0;
        while (fd <= 0) {
            sleep(5);
            fd = shm_open("posixshm", O_RDONLY, 0666);
            ii++;
            if (ii > 3)
                return -2;
        }
        char * pmp = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if (pmp) {
            printf("Child process reads message: %s\n", pmp);
            printf("Child process exits.\n");
        }
    }
    else if (ret > 0) { // parent process writes.
        int fd = shm_open("posixshm", O_CREAT | O_RDWR, 0666);
        if (fd <= 0) {
            assert(!"shm_open failed, how could it be...");
            return -1;
        }
        ftruncate(fd, SHM_SIZE);
        printf("Shm_open gets ready.\nParent process sends message to SHM.\n");
        char * pmp = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        memset(pmp, '\0', SHM_SIZE);
        char message[] = {"CadmanLin7"};
        memcpy(pmp, message, strlen(message));
        munmap(pmp, SHM_SIZE);
        printf("Message has been sent to SHM.\n");
        wait(NULL);
        printf("Parent process exits.\n");
    }
    return 0;
}
