/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2022.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* file_client.c
   Send a message to the server file_server.c requesting the
   contents of the file named on the command line, and then receive the
   file contents via a series of messages sent back by the server. Display
   the total number of bytes and messages received. The server and client
   communicate using System V message queues.
*/
#include "sfile.h"

static int clientId;

static void removeQueue(void){
    if (msgctl(clientId, IPC_RMID, NULL) == -1){
        perror("msgctl");
    }
}

int main(int argc, char *argv[]){
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0){
        fprintf(stderr,"%s pathname\n", argv[0]);
        exit(1);
    }

    if (strlen(argv[1]) > sizeof(req.pathname) - 1){
        fprintf(stderr,"pathname too long (max: %ld bytes)\n",
                (long) sizeof(req.pathname) - 1);
        exit(1);
    }
    /* Get server's queue identifier; create queue for response */

    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1){
        perror("msgget - server message queue");
        exit(1);
    }
    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1){
        perror("msgget - client message queue");
        exit(1);
    }
    if (atexit(removeQueue) != 0){
        perror("atexit");
        exit(1);
    }
    /* Send message asking for file named in argv[1] */

    req.mtype = 1;                      /* Any type will do */
    req.clientId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';
                                        /* Ensure string is terminated */

    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1){
        perror("msgsnd");
        exit(1);
    }
    /* Get first response, which may be failure notification */

    msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if (msgLen == -1){
        perror("msgrcv");
        exit(1);
    }
    if (resp.mtype == RESP_MT_FAILURE) {
        printf("%s\n", resp.data);      /* Display msg from server */
        exit(EXIT_FAILURE);
    }

    /* File was opened successfully by server; process messages
       (including the one already received) containing file data */

    totBytes = msgLen;                  /* Count first message */
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if (msgLen == -1){
            perror("msgrcv");
            exit(1);
        }
        totBytes += msgLen;
    }

    printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);

    exit(EXIT_SUCCESS);
}
