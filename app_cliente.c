#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "claves.h"

int main() {

    int res;
    char value1_out[256];
    int N_value2_out;
    float V_value2_out[32];
    struct Paquete value3_out;

    char clave1[256];
    char clave2[256];
    char clave3[256];
    char clave4[256];
    char value1_255[256];
    char value1_256[257];
    char key_256[257];
    float vector32[32];
    float vector1[1];

    struct Paquete paquete_max = {INT_MAX, INT_MAX, INT_MAX};
    struct Paquete paquete_min = {INT_MIN, INT_MIN, INT_MIN};

    // rellenar claves y value1 con el tamaño maximo permitido (255 caracteres)
    for(int i=0;i<255;i++){
        clave1[i]='A';
        clave2[i]='B';
        clave3[i]='C';
        clave4[i]='D';
        value1_255[i]='E';
    }

    // poner el caracter nulo al final de las cadenas para que sean validas
    clave1[255]='\0';
    clave2[255]='\0';
    clave3[255]='\0';
    clave4[255]='\0';
    value1_255[255]='\0';

    // rellenar claves y value1 con el tamaño maximo permitido + 1
    for(int i=0;i<256;i++){
        value1_256[i]='F';
        key_256[i]='G';
    }

    value1_256[256]='\0';
    key_256[256]='\0';

    // rellenar el vector de floats con valores de ejemplo
    for(int i=0;i<32;i++){
        vector32[i]=i+1;
    }

    vector1[0]=1.23;

    printf("\nINICIO DE PRUEBAS\n\n");
    
    
    printf("[TEST 1] destroy() -> Inicializar el servicio\n");
    res = destroy();
    printf("Resultado: %d\n\n", res);

    printf("[TEST 2] exist() -> Comprobar existencia de una clave inexistente\n");
    res = exist("clave_no_existe");
    printf("Resultado: %d\n\n", res);

    printf("[TEST 3] get_value() -> Obtener valores de una clave inexistente\n");
    res = get_value("clave_no_existe", value1_out, &N_value2_out, V_value2_out, &value3_out);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 4] modify_value() -> Modificar una clave inexistente\n");
    res = modify_value("clave_no_existe", value1_255, 1, vector1, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 5] delete_key() -> Borrar una clave inexistente\n");
    res = delete_key("clave_no_existe");
    printf("Resultado: %d\n\n", res);

    printf("[TEST 6] set_value() -> Insertar elemento valido con tamaños maximos\n");
    res = set_value(clave1, value1_255, 32, vector32, paquete_max);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 7] set_value() -> Insertar elemento valido con tamaño minimo del vector\n");
    res = set_value(clave2, value1_255, 1, vector1, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 8] exist() -> Comprobar que una clave insertada existe\n");
    res = exist(clave1);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 9] get_value() -> Recuperar valores de una clave existente\n");
    res = get_value(clave1, value1_out, &N_value2_out, V_value2_out, &value3_out);
    printf("Resultado: %d\n", res);

    if(res==0){
        printf("value1 recuperado: %s\n", value1_out);
        printf("Dimension del vector: %d\n", N_value2_out);
    }

    printf("\n");

    printf("[TEST 10] modify_value() -> Modificar valores de una clave existente\n");
    res = modify_value(clave1, value1_255, 32, vector32, paquete_max);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 11] set_value() -> Intentar insertar una clave duplicada\n");
    res = set_value(clave2, value1_255, 1, vector1, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 12] set_value() -> Insertar con tamaño de clave fuera de rango\n");
    res = set_value(key_256, value1_255, 1, vector1, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 13] set_value() -> Insertar con tamaño de value1 fuera de rango\n");
    res = set_value(clave3, value1_256, 1, vector1, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 14] set_value() -> Insertar con tamaño de vector fuera de rango superior\n");
    res = set_value(clave3, value1_255, 33, vector32, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 15] set_value() -> Insertar con tamaño de vector fuera de rango inferior\n");
    res = set_value(clave3, value1_255, 0, vector32, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 16] modify_value() -> Modificar con tamaño de value1 fuera de rango\n");
    res = modify_value(clave1, value1_256, 1, vector1, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 17] modify_value() -> Modificar con tamaño de vector fuera de rango superior\n");
    res = modify_value(clave1, value1_255, 33, vector32, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 18] modify_value() -> Modificar con tamaño de vector fuera de rango inferior\n");
    res = modify_value(clave1, value1_255, 0, vector32, paquete_min);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 19] delete_key() -> Borrar una clave existente\n");
    res = delete_key(clave1);
    printf("Resultado: %d\n\n", res);

    printf("[TEST 20] delete_key() -> Borrar una clave inexistente\n");
    res = delete_key(clave4);
    printf("Resultado: %d\n\n", res);

    printf("FIN DE PRUEBAS\n");

    return 0;
}