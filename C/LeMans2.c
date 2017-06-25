#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

void fahrManeuver(int ueberholen);

int main(int argc, char const *argv[]) {
  int i, shm_id, *rueckstand, rundenzahl = 14;
  int key = atoi(argv[1]);
  char* temp_s = "n";

  shm_id = shmget(key, sizeof(int), 0777);

  if (shm_id == -1) {
    printf("Das Segment konnte nicht angelegt werden!");
    exit(1);
  }

  rueckstand = (int *)shmat(shm_id, 0, 0);

  printf("Rückstand beträgt %i Sekunden.\n", *rueckstand);
  for (; rundenzahl > 0; rundenzahl--) {
    usleep(1000*500);

    //Richtige Einzahl
    if (rundenzahl == 1 )
       temp_s = "";
    printf("Noch %i Runde%s zu fahren\n", rundenzahl,temp_s);
    usleep(1000*500);

    if (*rueckstand -13 > 0) {
      fahrManeuver(0);
      *rueckstand -= 13;
      printf("\nRückstand beträgt noch %i Sekunden.\n", *rueckstand);
    }else{
      fahrManeuver(1);
      *rueckstand -= 13;
      printf("\nVorsprung beträgt %i Sekunden.\n", (*rueckstand * -1));
    }

    usleep(1000*500);
  }

  if (*rueckstand < 0)
    printf("Porsche 2 fährt als erster über die Ziellinie\n");
  else
    printf("Porsche 2 fährt als zweiter über die Ziellinie\n");

  return 0;
}

// Schöne Ausgabe
void fahrManeuver(int ueberholen){
  int i;
  if (ueberholen == 1) {
    printf("Porsche 2 überholt");
  }else{
    printf("Porsche 2 holt auf");
  }
  fflush(stdout);
  for (i = 0; i < 3; i++) {
    usleep(1000*500);
    printf(".");
    fflush(stdout);
  }
    usleep(1000*500);
}
