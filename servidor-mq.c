#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "mensaje.h"
#include "claves.h"

// en el enunciado no dice el maximo de peticiones que se puede tener en cola, asi que lo fijamos arbitrariamente
#define MAX_PETICIONES_EN_COLA 10


// SERVIDOR_1993 para evitar que coincida el nombre con otros servidores
// se han combinado los ultimos dos digitos de los NIA del equipo: 100522219 y 100522293, por lo que es 1993
#define COLA_SERVIDOR "/SERVIDOR_1993"

/* mutex y var cond para la copia del mensaje*/
pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado = true;
pthread_cond_t cond_mensaje;
mqd_t  q_servidor;

/* mutex para las operaciones */
pthread_mutex_t mutex_operaciones;


void manejador_sigint(int sig) {
    printf("\n[SERVIDOR] Capturada senyal %d (Ctrl+C). Limpiando recursos...\n", sig);
    
    // llamamos a destroy() para liberar la lista enlazada
    if (destroy() == 0) {
        printf("[SERVIDOR] Lista enlazada liberada con éxito.\n");
    }
    
    // cerramos y eliminamos la cola del servidor
    mq_close(q_servidor);
    mq_unlink(COLA_SERVIDOR);
    
    printf("[SERVIDOR] Cola cerrada y eliminada.\n");
    exit(0); 
}


void tratar_mensaje(void  *mess){

    // para almacenar la copia local del mensaje dentro de la funcion
	struct peticion mensaje;

    // cola del cliente
	mqd_t q_cliente;

    // respuesta al cliente: int resultado; char value1[256]; int N_value2; float V_value2[MAXN]; struct Paquete value3; 
	struct respuesta res;

	// copia del mensaje a la estructura local en la funcion
	pthread_mutex_lock(&mutex_mensaje);

	mensaje = (*(struct peticion *) mess);

	// se marca como copiado y se puede despertar para atender mas peticiones
	mensaje_no_copiado = false;
    // despierta un hilo
	pthread_cond_signal(&cond_mensaje);
	pthread_mutex_unlock(&mutex_mensaje);

    // obtenemos el pid del cliente a partir del nombre de su cola para los prints que sirven a modo de log
    int cliente_pid;
    sscanf(mensaje.q_name, "/cola_cliente_%d", &cliente_pid);

	// se ejecuta la peticion para mandar la respuesta
    pthread_mutex_lock(&mutex_operaciones);
    switch(mensaje.op) {
        case OP_DESTROY:
            res.resultado = destroy();
            printf("[SERVIDOR] Operación destroy realizada con código de resultado %d para el cliente %d\n", res.resultado, cliente_pid);
            break;
        case OP_SET:
            res.resultado = set_value(mensaje.key, mensaje.value1, mensaje.N_value2, mensaje.V_value2, mensaje.value3);
            printf("[SERVIDOR] Operación set_value realizada con código de resultado %d para el cliente %d\n", res.resultado, cliente_pid);
            break;
        case OP_GET:
            res.resultado = get_value(mensaje.key, res.value1, &res.N_value2, res.V_value2, &res.value3);
            printf("[SERVIDOR] Operación get_value realizada con código de resultado %d para el cliente %d\n", res.resultado, cliente_pid);
            break;
        case OP_MODIFY:
            res.resultado = modify_value(mensaje.key, mensaje.value1, mensaje.N_value2, mensaje.V_value2, mensaje.value3);
            printf("[SERVIDOR] Operación modify_value realizada con código de resultado %d para el cliente %d\n", res.resultado, cliente_pid);
            break;
        case OP_DELETE:
            res.resultado = delete_key(mensaje.key);
            printf("[SERVIDOR] Operación delete_key realizada con código de resultado %d para el cliente %d\n", res.resultado, cliente_pid);
            break;
        case OP_EXIST:
            res.resultado = exist(mensaje.key);
            printf("[SERVIDOR] Operación exist realizada con código de resultado %d para el cliente %d\n", res.resultado, cliente_pid);
            break;
        default:
            perror("Operacion inválida");
            // se devuelve el codigo de error para el servicio de tuplas
            res.resultado = -1;
    }
    
    pthread_mutex_unlock(&mutex_operaciones);


    // devolver resultado al cliente
	// se abre la cola del cliente primero
	q_cliente = mq_open(mensaje.q_name, O_WRONLY);

    // captura de error si mq_open devuelve -1
	if (q_cliente == -1){
		perror("No se puede abrir la cola del cliente");
	}
	else {
        // si se ha podido abrir la cola del cliente, se pone la respuesta en la cola
		if (mq_send(q_cliente, (const char *) &res, sizeof(struct respuesta), 0) < 0) {
			perror("Error en mq_send");
            // se cierra la cola del cliente al acabar
			mq_close(q_cliente);
		}
	}
    mq_close(q_cliente);
	pthread_exit(0);
}


int main(void) {

    // como acabamos el proceso servidor con ctrl+c (SIGINT), usamos un manejador de la senyal
    // de modo que se limpie adecuadamente la memoria y las colas
    signal(SIGINT, manejador_sigint);
    
    struct peticion mess;     
    struct mq_attr attr;
	pthread_attr_t t_attr;		// atributos de los threads 
   	pthread_t thid;

    // este atributo fija el maximo de peticiones que se puede tener en cola
    attr.mq_maxmsg = MAX_PETICIONES_EN_COLA;                
	attr.mq_msgsize = sizeof(struct peticion);

    // SERVIDOR_1993 para evitar que coincida el nombre con otros servidores
    // se han combinado los ultimos dos digitos de los NIA del equipo: 100522219 y 100522293, por lo que es 1993
    q_servidor = mq_open(COLA_SERVIDOR, O_CREAT|O_RDONLY, 0700, &attr);
	if (q_servidor == -1) {
		perror("mq_open");
		return -1;
	}

    // inicializar mutex y var cond
	pthread_mutex_init(&mutex_mensaje, NULL);
	pthread_cond_init(&cond_mensaje, NULL);
	pthread_attr_init(&t_attr);

    pthread_mutex_init(&mutex_operaciones, NULL);


	// atributos de los threads, threads independientes
	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    // bucle infinito, en principio no hay una condicion de salida definida (se podra usar ctrl+C al probar cuando se quiere finalizar el proceso)
    while(1) {
        // recibe peticion
        if (mq_receive(q_servidor, (char *) &mess, sizeof(mess), 0) < 0 ) {
            perror("mq_recev");
            return -1;
        }

        // se crea un hilo para atender la peticion
        if (pthread_create(&thid, &t_attr, (void *)tratar_mensaje, (void *)&mess)== 0) {
            // se espera a que el thread copie el mensaje 
            pthread_mutex_lock(&mutex_mensaje);
            while (mensaje_no_copiado)
                pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
            mensaje_no_copiado = true;
            pthread_mutex_unlock(&mutex_mensaje);
        }   
    }

    // en principio no se llega aqui por el bucle infinito, pero si se llegara seria necesario lo que viene a continuzacion para limpiar
    pthread_cond_destroy(&cond_mensaje);
    pthread_mutex_destroy(&mutex_mensaje);
    pthread_mutex_destroy(&mutex_operaciones);
    mq_close(q_servidor);
    mq_unlink(COLA_SERVIDOR);

	return 0;
}