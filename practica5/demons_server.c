#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define SERVER_QUEUE_NAME "/demons_server_queue"
#define MAX_SIZE 1024
#define MSG_STOP "STOP"

struct requestMsg {
    char pathname[MAX_SIZE];
    char clientQueueName[64];
};

struct responseMsg {
    char data[MAX_SIZE];
};

static void grimReaper(int sig) { /* SIGCHLD handler */
    int savedErrno;

    savedErrno = errno;                 /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

static void serveRequest(const struct requestMsg *req) {
    /* Executed in child process: serve a single client */
    int fd;
    ssize_t numRead;
    mqd_t clientQueue;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {                     /* Open failed: send error text */
        resp.data[0] = '\0';
        strncat(resp.data, "Couldn't open", MAX_SIZE - 1);
        clientQueue = mq_open(req->clientQueueName, O_WRONLY);
        if (clientQueue != (mqd_t) -1) {
            mq_send(clientQueue, (const char *)&resp, sizeof(resp), 0);
            mq_close(clientQueue);
        }
        exit(EXIT_FAILURE);             /* and terminate */
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
       diagnose read() and mq_send() errors since we can't notify client. */

    while ((numRead = read(fd, resp.data, MAX_SIZE)) > 0) {
        clientQueue = mq_open(req->clientQueueName, O_WRONLY);
        if (clientQueue == (mqd_t) -1) {
            close(fd);
            exit(EXIT_FAILURE);
        }
        mq_send(clientQueue, (const char *)&resp, numRead, 0);
        mq_close(clientQueue);
    }

    /* Send a message of type MSG_STOP to signify end-of-file */
    clientQueue = mq_open(req->clientQueueName, O_WRONLY);
    if (clientQueue != (mqd_t) -1) {
        strcpy(resp.data, MSG_STOP);
        mq_send(clientQueue, (const char *)&resp, sizeof(resp), 0);
        mq_close(clientQueue);
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    mqd_t serverQueue;
    struct sigaction sa;

    /* Create server message queue */
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct requestMsg);
    attr.mq_curmsgs = 0;

    serverQueue = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (serverQueue == (mqd_t) -1) {
        perror("mq_open - server queue");
        exit(EXIT_FAILURE);
    }

    /* Establish SIGCHLD handler to reap terminated children */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Read requests, handle each in a separate child process */
    for (;;) {
        msgLen = mq_receive(serverQueue, (char *)&req, sizeof(req), NULL);
        if (msgLen == -1) {
            if (errno == EINTR)         /* Interrupted by SIGCHLD handler? */
                continue;               /* ... then restart mq_receive() */
            perror("mq_receive");       /* Some other error */
            break;                      /* ... so terminate loop */
        }

        pid = fork();                   /* Create child process */
        if (pid == -1) {
            perror("fork");
            break;
        }

        if (pid == 0) {                 /* Child handles request */
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    /* If mq_receive() or fork() fails, remove server MQ and exit */
    mq_unlink(SERVER_QUEUE_NAME);
    exit(EXIT_FAILURE);
}
