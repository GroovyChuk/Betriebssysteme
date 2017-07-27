#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SEGSIZE sizeof(int)
#define NUM_OF_CHILDS 4
#define MAXCOUNT 100000

int main(){

  int i, shm_id, sem_id, *shar_mem, pid[NUM_OF_CHILDS];
  unsigned short marker[1];

  sem_id = semget(IPC_PRIVATE,1,IPC_CREAT|0644);

  marker[0] = 1;
  semctl(sem_id, 1, SETALL, marker);

  shm_id = shmget(IPC_PRIVATE, SEGSIZE, IPC_CREAT|0644);

  shar_mem = (int *)shmat(shm_id, 0, 0);
  *shar_mem = 0;

  for (i = 0; i < NUM_OF_CHILDS; i++) {
    // fork n children
    pid[i] = fork();
    if (pid[i] == -1)
      exit(1);

      if (pid[i] == 0){
        int count = 0;
        struct sembuf enter, leave;
        // prepare up & down for semop
        enter.sem_num = 0;
        enter.sem_op = -1;
        enter.sem_flg = SEM_UNDO;
        leave.sem_num = 0;
        leave.sem_op = 1;
        leave.sem_flg = SEM_UNDO;

        //enter critical section
        semop(sem_id,&enter,1);
        while (*shar_mem < MAXCOUNT) {
          *shar_mem += 1;
          count++;
        }

        printf("Es wurden %i Durchläufe benötigt.\n", count);
        //leave critical section
        semop(sem_id,&leave,1);
        exit(0);
      }
  }

  for (i = 0; i < NUM_OF_CHILDS; i++){
    // wait for death
    waitpid(pid[i], NULL, 0);
  }

  return 0;
}
