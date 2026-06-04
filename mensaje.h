#ifndef MENSAJE_H
#define MENSAJE_H

#include "claves.h"

#define MAXSIZE	256
#define MAX_QUEUE_NAME 256
#define MAXN 32

// operaciones posibles
#define OP_DESTROY 0
#define OP_SET     1
#define OP_GET     2
#define OP_MODIFY  3
#define OP_DELETE  4
#define OP_EXIST   5


// peticion que cliente envia a servidor
struct peticion {

    int op;

    char key[MAXSIZE];
    char value1[MAXSIZE];

    int N_value2;
    float V_value2[MAXN];

    struct Paquete value3;

    char q_name[MAX_QUEUE_NAME];  // cola del cliente
};


// mensaje que el servidor devuelve
struct respuesta {

    int resultado;

    char value1[MAXSIZE];
    int N_value2;
    float V_value2[MAXN];

    struct Paquete value3;
};

#endif