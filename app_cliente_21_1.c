#include <stdio.h>
#include <string.h>
#include "claves.h"

int main() {
    // cliente 1 inserta la clave
    char key[256] = "Prueba21";
    int res;
    char value1[] = "value1prueba21";
    int N_value2 = 1;
    float V_value2[] = {1.1};
    struct Paquete value3 = {0,0,0};

    // escritura
    res = set_value(key, value1, N_value2, V_value2, value3);

    printf("[CLIENTE 1] Ha insertado la clave \"Prueba21\" con código %d.\n", res);
    return 0;
}