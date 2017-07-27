#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

struct text_message {
  long mtype;
  char mtext[100];
};

int main(int argc, char *argv[]){
  int msid, v;
  struct text_message mess;
  if (argc != 3) {
    printf("usage: msgrcv <key> <type>\n");
    exit(1);
  }

/* Get the message queue, do not create it */
  msid = msgget((key_t)atoi(argv[1]), 0);
  if (msid == -1) {
    printf("cannot get message queue\n");
    exit(1);
  }
/* Read a message of the specified type, do not block */
  v = msgrcv(msid, &mess, 100, atoi(argv[2]), IPC_NOWAIT);
  if ((v<0)) {
    if (errno==ENOMSG)
      printf("no appropriate message on queue\n");
    else
      printf("error reading from queue\n");
  } else
    printf("[%d] %s\n", (int) mess.mtype, mess.mtext);

    return 0;
}
