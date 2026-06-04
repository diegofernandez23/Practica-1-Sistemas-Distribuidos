#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "claves.h"
#include "mensaje.h"

// nombre de la cola servidor (explicado en servidor-mq.c)
#define SERVER_QUEUE "/SERVIDOR_1993"

// funcion auxiliar para enviar una peticion
// se asume que los campos de la peticion (salvo q_name) se han rellenado previamente en la funcion que llama a esta auxiliar
static int enviar_peticion(struct peticion *pet, struct respuesta *res) {
    // creamos las colas (sin inicializar aun)
    mqd_t q_servidor;
    mqd_t q_cliente;
    char queuename[MAX_QUEUE_NAME];
    struct mq_attr attr;

    // fijamos el numero maximo de peticiones en cola a 10 (arbitrario), y el mensaje tiene el tamanyo de la estructura de peticion
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct respuesta);

    // nombre cola del cliente, unico con el pid
    sprintf(queuename, "/cola_cliente_%d", getpid());
    // copiamos el nombre de la cola a la peticion que vamos a enviar
    strcpy(pet->q_name, queuename);


    // abrir cola cliente
    q_cliente = mq_open(queuename, O_CREAT | O_RDONLY, 0700, &attr);
    if (q_cliente == -1) {
        perror("Error al abrir cola cliente");
        return -2; // error de comunicaciones
    }

    // abrir cola servidor
    q_servidor = mq_open(SERVER_QUEUE, O_WRONLY);
    if (q_servidor == -1) {
        perror("Error al abrir cola servidor");
        // si ha habido error de comunicaciones, cerramos y eliminamos la del cliente, dado que este proceso acabara con error
        mq_close(q_cliente);
        mq_unlink(queuename); 
        return -2;
    }

    // enviar peticion
    if (mq_send(q_servidor, (const char *)pet, sizeof(struct peticion), 0) == -1) {
        perror("Error mq_send");
        // si ha habido error de comunicaciones, cerramos y eliminamos la del cliente, dado que este proceso acabara con error
        mq_close(q_servidor);
        mq_close(q_cliente);
        mq_unlink(queuename);
        return -2;
    }

    // recibir respuesta
    if (mq_receive(q_cliente, (char *)res, sizeof(struct respuesta), 0) == -1) {
        perror("Error mq_receive");
        // si ha habido error de comunicaciones, cerramos y eliminamos la del cliente, dado que este proceso acabara con error
        mq_close(q_servidor);
        mq_close(q_cliente);
        mq_unlink(queuename);
        return -2;
    }

    // cerramos las colas al acabar: se ha enviado la peticion y se ha recibido la respuesta correctamente
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(queuename);

    return 0; // todo ok
}

// funcion que llama el cliente cuando quiere hacer la operacion destroy
// se encarga de generar la peticion para enviar al servidor
int destroy(void) {
    struct peticion pet;
    struct respuesta res;
    pet.op = OP_DESTROY;
    
    // llama a la funcion auxiliar para enviar la peticion
    // se devuelve -2 (error de comunicaciones) si hay un error en el envio de la peticion
    if (enviar_peticion(&pet, &res) != 0) {
        return -2;
    }
    return res.resultado;
}

// funcion que llama el cliente cuando quiere hacer la operacion set_value
// se encarga de generar la peticion para enviar al servidor
int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // validacion local de la longitud de los parametros
    if (key == NULL || value1 == NULL || V_value2 == NULL || 
        strlen(key) > 255 || strlen(value1) > 255 || N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    struct peticion pet;
    struct respuesta res;
    pet.op = OP_SET;
    // copia key y value1 a la peticion
    strcpy(pet.key, key);
    strcpy(pet.value1, value1);

    // copia los campos de N y V value2, y value3
    pet.N_value2 = N_value2;
    for(int i=0; i<N_value2; i++) {
        pet.V_value2[i] = V_value2[i];
    }
    pet.value3 = value3;

    // llama a la funcion auxiliar para enviar la peticion
    // se devuelve -2 (error de comunicaciones) si hay un error en el envio de la peticion
    if (enviar_peticion(&pet, &res) != 0) {
        return -2;
    }
    return res.resultado;
}

// funcion que llama el cliente cuando quiere hacer la operacion get_value
// se encarga de generar la peticion para enviar al servidor
int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    // validacion local de la longitud de parametro (solo key porque los otros argumentos son para devolver en puntero)
    if (key == NULL || strlen(key) > 255) {
        return -1;
    }

    struct peticion pet;
    struct respuesta res;
    pet.op = OP_GET;
    // copia la clave a la peticion
    strcpy(pet.key, key);

    // envia peticion con la funcion auxiliar
    // se devuelve -2 (error de comunicaciones) si hay un error en el envio de la peticion
    if (enviar_peticion(&pet, &res) != 0) {
        return -2;
    }

    // el servidor devuelve 0 si se ha insertado con exito (res.resultado)
    if (res.resultado == 0) {
        // se copian los campos de la estructura respuesta a los campos dados como parametros (punteros)
        // asi el cliente ve la respuesta en las propias variables como si hubiera hecho la peticion directamente (no conoce la implementacion)
        strcpy(value1, res.value1);
        *N_value2 = res.N_value2;
        for(int i=0; i<res.N_value2; i++) {
            V_value2[i] = res.V_value2[i];
        }
        *value3 = res.value3;
    }
    return res.resultado;
}

// funcion que llama el cliente cuando quiere hacer la operacion modify_value
// se encarga de generar la peticion para enviar al servidor
int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // validacion local de la longitud de los parametros, como en el set
    if (key == NULL || value1 == NULL || V_value2 == NULL || 
        strlen(key) > 255 || strlen(value1) > 255 || N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    struct peticion pet;
    struct respuesta res;
    pet.op = OP_MODIFY;

    // copia de los argumentos a la peticion
    strcpy(pet.key, key);
    strcpy(pet.value1, value1);
    pet.N_value2 = N_value2;
    for(int i=0; i<N_value2; i++) {
        pet.V_value2[i] = V_value2[i];
    }
    pet.value3 = value3;

    // envia la peticion con la funcion aixiliar
    // se devuelve -2 (error de comunicaciones) si hay un error en el envio de la peticion
    if (enviar_peticion(&pet, &res) != 0) {
        return -2;
    }
    return res.resultado;
}

// funcion que llama el cliente cuando quiere hacer la operacion delete_key
// se encarga de generar la peticion para enviar al servidor
int delete_key(char *key) {
    // validacion local del argumento
    if (key == NULL || strlen(key) > 255) {
        return -1;
    }

    struct peticion pet;
    struct respuesta res;
    // copia del argumento y el operador
    pet.op = OP_DELETE;
    strcpy(pet.key, key);

    // llamada a la funcion auxiliar para enviar la peticion
    // se devuelve -2 (error de comunicaciones) si hay un error en el envio de la peticion
    if (enviar_peticion(&pet, &res) != 0) {
        return -2;
    }
    return res.resultado;
}

// funcion que llama el cliente cuando quiere hacer la operacion exist
// se encarga de generar la peticion para enviar al servidor
int exist(char *key) {
    // validacion local del argumento
    if (key == NULL || strlen(key) > 255) {
        return -1;
    }

    struct peticion pet;
    struct respuesta res;
    // copia del argumento y el operador
    pet.op = OP_EXIST;
    strcpy(pet.key, key);

    // llamada a la funcion auxiliar para enviar la peticion
    // se devuelve -2 (error de comunicaciones) si hay un error en el envio de la peticion
    if (enviar_peticion(&pet, &res) != 0) {
        return -2;
    }
    return res.resultado;
}