#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "claves.h"

int main() {
    // igual para todos los clientes
    char key[256] = "colision";
    int pid = getpid();

    // cada cliente espera hasta 10000 microsegundos al azar
    usleep(rand() % 10000);

    // escritura
    set_value(key, "prueba24", 1, (float[]){1.1}, (struct Paquete){0,0,0});

    printf("[CLIENTE %d] Finalizado.\n", pid);
    return 0;
}