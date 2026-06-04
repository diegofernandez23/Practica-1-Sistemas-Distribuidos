#include <stdio.h>
#include <string.h>
#include "claves.h"

int main() {
    // cliente 1 inserta la clave
    char key[256] = "Prueba21";

    // valores introducidos por el cliente 1
    char value1[] = "value1prueba21";
    int N_value2 = 1;
    float V_value2[] = {1.1};
    struct Paquete value3 = {0,0,0};

    // salida
    int res;
    char value1_out[256];
    int N_value2_out = 0;
    float V_value2_out[32];
    struct Paquete value3_out;

    // escritura
    res = get_value(key, value1_out, &N_value2_out, V_value2_out, &value3_out);

    printf("[CLIENTE 1] Ha recuperado la clave \"Prueba21\" con código %d.\n", res);

    // comprobamos que los valores recuperados son iguales a los que habia introducido el cliente 1
    // como el array solo tenia un elemento, directamente lo comparamos, en vez de usar un bucle
    if (strcmp(value1, value1_out) == 0 && N_value2 == N_value2_out && V_value2[0] == V_value2_out[0] 
        && value3.x == value3_out.x && value3.y == value3_out.y && value3.z == value3_out.z) {
        printf("[CLIENTE 1] Coinciden todos los valores recuperados.\n");
    }
    
    return 0;
}