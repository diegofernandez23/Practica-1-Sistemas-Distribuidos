#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "claves.h"

int main() {
    char key[256];
    int pid = getpid();

    // nombre de claves que usa el cliente
    // pid para que no coincida con el nombre de clave que usen otros clientes
    sprintf(key, "CONCURRENTE_%d", pid);

    for (int i = 0; i < 8; i++) {
        printf("[CLIENTE LENTO %d] Iteracion %d\n", pid, i);

        // escritura
        set_value(key, "datos_iniciales", 1, (float[]){1.1}, (struct Paquete){0,0,0});
        sleep(1); // espera un segundo
        
        // limpieza
        delete_key(key);
        sleep(1);
    }

    printf("[CLIENTE LENTO %d] Finalizado.\n", pid);
    return 0;
}