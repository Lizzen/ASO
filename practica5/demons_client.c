#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <errno.h>

#define SERVER_QUEUE_NAME "/demons_server_queue"
#define CLIENT_QUEUE_NAME_TEMPLATE "/demons_client_queue_%d"
#define MAX_SIZE 1024
#define MSG_STOP "STOP"

struct requestMsg {
    char pathname[MAX_SIZE];
};

struct responseMsg {
    char data[MAX_SIZE];
};

static mqd_t clientQueue;
static char clientQueueName[64];

static void removeQueue(void) {
    if (mq_unlink(clientQueueName) == -1) {
        perror("mq_unlink");
    }
}

int main(int argc, char *argv[]) {
    struct requestMsg req;
    struct responseMsg resp;
    mqd_t serverQueue;
    ssize_t msgLen;
    size_t totBytes;
    unsigned int prio;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) >= MAX_SIZE) {
        fprintf(stderr, "pathname too long (max: %ld bytes)\n", (long)MAX_SIZE - 1);
        exit(EXIT_FAILURE);
    }

    snprintf(clientQueueName, 64, CLIENT_QUEUE_NAME_TEMPLATE, getpid());

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct responseMsg);
    attr.mq_curmsgs = 0;

    clientQueue = mq_open(clientQueueName, O_CREAT | O_RDONLY, 0644, &attr);
    if (clientQueue == (mqd_t)-1) {
        perror("mq_open - client queue");
        exit(EXIT_FAILURE);
    }

    if (atexit(removeQueue) != 0) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    serverQueue = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (serverQueue == (mqd_t)-1) {
        perror("mq_open - server queue");
        exit(EXIT_FAILURE);
    }

    strncpy(req.pathname, argv[1], MAX_SIZE);
    if (mq_send(serverQueue, (const char*)&req, sizeof(req), 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    totBytes = 0;
    while (1) {
        msgLen = mq_receive(clientQueue, (char*)&resp, sizeof(resp), &prio);
        if (msgLen == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        if (strcmp(resp.data, MSG_STOP) == 0) {
            break;
        }

        printf("%s", resp.data);
        totBytes += msgLen;
    }

    printf("Received %ld bytes\n", (long)totBytes);

    exit(EXIT_SUCCESS);
}
