#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "claves.h"

int main() {
    char key[256];
    float v2[3] = {1.0, 2.0, 3.0};
    struct Paquete p = {1, 2, 3};
    int pid = getpid();

    for (int j = 0; j < 5; j++) {
        printf("[CLIENTE RAPIDO %d] Iniciando nueva rafaga de peticiones...\n", pid);
        for (int i = 0; i < 100; i++) {
            sprintf(key, "KEY_%d_%d", pid, i);
            
            // insertar
            if (set_value(key, "valor_estres", 3, v2, p) == 0) {
                // Pequenya pausa para no saturar instantaneamente y facilitar que otros entren
                usleep(10000); // 10 milisegundos
                
                // verificar existencia
                exist(key);
                
                // borrar para no llenar la memoria del servidor infinitamente
                delete_key(key);
            }
        }
        // espera hasta 5 al azar antes de la proxima rafaga
        sleep(rand() % 6);
    }

    printf("[CLIENTE RAPIDO %d] Finalizado.\n", pid);
    return 0;
}