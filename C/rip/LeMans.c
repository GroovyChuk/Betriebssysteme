#include <stdio.h>
#include "string.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXCOUNT 1000000
#define NUM_OF_CHILDS 2

int main(int argc, char const *argv[]) {
  int i, shm_id, *rueckstand, key = 80;

  key = atoi(argv[1]);


  shm_id = shmget(key, sizeof(int), IPC_CREAT|0777);

  if (shm_id == -1) {
      printf("Das Segment konnte nicht angelegt werden!");
    exit(1);
  }

  rueckstand = (int *)shmat(shm_id, 0, 0);
  *rueckstand = 180;

  printf("Key: %i\nShared Zeit: %i sek.\n", key, *rueckstand);

  return 0;
}
