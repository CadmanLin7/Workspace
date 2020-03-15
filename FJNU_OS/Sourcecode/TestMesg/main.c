#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#define TYPE_SERVER 1
#define TYPE_CLIENT 2
#define TEXT_SIZE 1024

extern int CreateMsgQueue();
extern int GetMsgQueue();
extern int DestoryMsgQueue(int msg_id);
extern int SendMsgQueue(int msg_id, int who, char* msg);
extern int RecvMsgQueue(int msg_id, int who, char* msg);

static int commMsgQueue(const int flags) {
    key_t key = ftok("/tmp", 0x6666);
    if (key < 0) {
        perror("ftok failed.\n");
        return -1;
    }
    int msg_id = msgget(key, flags);
    if (msg_id < 0) {
        perror("msgget failed.\n");
        return -2;
    }
    return msg_id;
}

typedef struct msgbuf {
    long mtype;
    char mtext[TEXT_SIZE];
} MSG;

int main()
{
    pid_t pid = 0;

    pid = fork();
    if (pid > 0) { // parent process == server
        int msg_id = CreateMsgQueue();

        char buf[TEXT_SIZE];
        int ii = 3;
        memset(buf, '\0', TEXT_SIZE*sizeof(char));
        while (ii > 0) {
            RecvMsgQueue(msg_id, TYPE_CLIENT, buf);
            printf("Server: Client says, %s.\n", buf);
            printf("Server: Input message body.\n");
            fflush(stdout);
            ssize_t s = read(0, buf, sizeof(buf));
            if (s > 0) {
                buf[s-1] = '\0';
                SendMsgQueue(msg_id, TYPE_SERVER, buf);
                printf("Server: Sending message. [OK]\n");
            }
            ii--;
        }
        wait(NULL);
        DestoryMsgQueue(msg_id);
        printf("Parent process exits.\n");
    }
    else if (pid == 0) {
        int msg_id = GetMsgQueue();
        char buf[TEXT_SIZE];
        memset(buf, '\0', TEXT_SIZE*sizeof(char));
        int ii = 3;
        while (ii > 0) {
            printf("Client: Input message body.\n");
            fflush(stdout);
            ssize_t s = read(0, buf, sizeof(buf));
            if (s > 0) {
                buf[s-1] = '\0';
                SendMsgQueue(msg_id, TYPE_CLIENT, buf);
                printf("Client: Sending message. [OK]\n");
            }
            RecvMsgQueue(msg_id, TYPE_SERVER, buf);
            printf("Client: Server says, %s.\n", buf);
            ii--;
        }
        printf("Child process exits.\n");
    }
    return 0;
}

int CreateMsgQueue() {
    return commMsgQueue(IPC_CREAT | IPC_EXCL | 0666);
}

int GetMsgQueue() {
    return commMsgQueue(IPC_CREAT);
}

int DestoryMsgQueue(int msg_id) {
    if (msgctl(msg_id, IPC_RMID, NULL) < 0) {
        perror("msgdestory failed.\n");
        return -1;
    }
    return 0;
}

int SendMsgQueue(int msg_id, int who, char* msg) {
    MSG buf;
    buf.mtype = who;
    strcpy(buf.mtext, msg);

    if (msgsnd(msg_id, &buf, sizeof(buf.mtext), 0) < 0) {
        perror("msgsnd failed.\n");
        return -1;
    }
    return 0;
}

int RecvMsgQueue(int msg_id, int who, char* msg) {
    MSG buf;

    if (msgrcv(msg_id, &buf, sizeof(buf.mtext), who, 0) < 0) {
        perror("msgrcv failed.\n");
        return -1;
    }
    strncpy(msg, buf.mtext, sizeof(buf.mtext));
    return 0;
}
