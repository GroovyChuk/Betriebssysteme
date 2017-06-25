/*******************************************************************
* no_race.c   Weiterentwicklung von race.c                       *
* Mehrere Kindprozesse greifen auf ein gemeinsames         Segment
*
* zu. Das Programm zeigt die Verwendung von Semaphoren.           *
* Dabei wird das in race.cc auftretende gegenseitige Überschreiben
*
* des Speichers ebenso unterbunden wie das Vermischen der Ausgabe.
*
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>         // Headerfile für Semaphore
#include <sys/wait.h>
#define MAXCOUNT 1000000
#define NUM_OF_CHILDS 4
#define SEGSIZE sizeof(int)

int main() {
  // Variable sem_id für die Semaphorengruppe und
  // aus technischen Gründen eine Variable marker[1].
  int i, shm_id, sem_id, *shar_mem, pid[NUM_OF_CHILDS];
  unsigned short marker[1];
  // Es folgt das Anlegen der Semaphorengruppe. Es wird hier nur ein
  // Semaphor erzeugt. Anschließend wird der Semaphor auf 1 gesetzt.
  sem_id = semget (IPC_PRIVATE, 1, IPC_CREAT|0644);
  if (sem_id == -1) {
    printf("Die Gruppe konnte nicht angelegt werden!");
    exit(1);
  }
  marker[0] = 1;
  semctl(sem_id, 1, SETALL, marker);

  // Anforderung des Shared Memory Segments
  shm_id = shmget(IPC_PRIVATE, SEGSIZE, IPC_CREAT|0644);
  if (shm_id == -1) {
    printf("Das Segment konnte nicht angelegt werden!");
    exit(1);
  }
  shar_mem = (int *)shmat(shm_id, 0, 0);
  *shar_mem = 0;
  // Der Vaterprozeß erzeugt eine bestimmte Anzahl Kindprozesse.
  for (i = 0; i < NUM_OF_CHILDS; i++) {
    pid[i] = fork();
    if (pid[i] == -1){
      printf("Kind konnte nicht erzeugt werden!");
      exit(1);
    }
  }

  if (pid[i] == 0){
      int count = 0;
      struct sembuf enter, leave; // Structs für den Semaphor
      enter.sem_num = 0;    // Semaphor 0 in der Gruppe
      enter.sem_op = -1;    // blockieren
      enter.sem_flg = SEM_UNDO;
      leave.sem_num = 0;    // Semaphor 0 in der Gruppe
      leave.sem_op = 1;     // freigeben
      leave.sem_flg = SEM_UNDO;
      semop(sem_id,&enter,1); // Eintritt in kritischen Bereich
      while (*shar_mem < MAXCOUNT) {
             *shar_mem += 1;
                  count++;
           }
      printf("Es wurden %i Durchläufe benötigt.", count);
      semop(sem_id,&leave,1); // Verlassen des krit. Bereichs
      exit(0);
    }

    // Der Vaterprozess wartet, bis alle Kindprozesse beendet sind.
    for (i = 0; i < NUM_OF_CHILDS; i++)
    waitpid(pid[i], NULL, 0);
    // Die Semaphorengruppe und das Shared Memory Segment werden
    // wieder freigegeben.
    shmdt(shar_mem);
    shmctl(sem_id, IPC_RMID, 0);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}
