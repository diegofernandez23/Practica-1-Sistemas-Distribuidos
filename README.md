# Práctica 1 - Sistemas Distribuidos (3º curso)

## Descripción

Proyecto desarrollado para la asignatura de **Sistemas Distribuidos** cuyo objetivo es implementar un servicio distribuido de almacenamiento clave-valor utilizando bibliotecas dinámicas, colas de mensajes POSIX y comunicación cliente-servidor.

La práctica se divide en dos partes:

* **Parte A:** implementación local del servicio mediante una biblioteca dinámica.
* **Parte B:** implementación distribuida utilizando una arquitectura cliente-servidor basada en colas de mensajes POSIX.

## Autores

* Diego Fernández González
* Blanca Peña Moñino

## Tecnologías utilizadas

* Lenguaje C
* GCC
* Bibliotecas dinámicas (`.so`)
* POSIX Message Queues
* POSIX Threads
* Linux

## Funcionalidades

El servicio permite gestionar elementos de la forma:

```text
<key, value1, value2, value3>
```

donde:

* `key`: clave identificadora.
* `value1`: cadena de texto.
* `value2`: vector de números reales.
* `value3`: estructura `Paquete`.

Las operaciones soportadas son:

* Inserción de elementos (`set_value`)
* Consulta de elementos (`get_value`)
* Modificación de elementos (`modify_value`)
* Eliminación de elementos (`delete_key`)
* Comprobación de existencia (`exist`)
* Reinicio completo del servicio (`destroy`)

## Estructura del proyecto

```text
.
├── claves.c
├── claves.h
├── mensaje.h
├── proxy-mq.c
├── servidor-mq.c
├── app_cliente.c
├── app_cliente_21_1.c
├── app_cliente_21_2.c
├── app_cliente_23_lento.c
├── app_cliente_23_rapido.c
├── app_cliente_24.c
├── app-cliente-21.sh
├── app-cliente-23.sh
├── app-cliente-24.sh
└── Makefile
```

## Compilación

### Parte A

Compila la versión local del servicio:

```bash
make parteA
```

Se generarán:

```text
libclaves.so
app_cliente_A
```

### Parte B

Compila la versión distribuida:

```bash
make parteB
```

Se generarán:

```text
servidor
app_cliente_B
libproxyclaves.so
```

además de los distintos programas de prueba incluidos en el proyecto.

## Ejecución

### Iniciar el servidor

```bash
./servidor
```

### Ejecutar un cliente

```bash
./app_cliente_B
```

## Arquitectura

La versión distribuida sigue una arquitectura cliente-servidor:

1. El cliente invoca operaciones sobre la biblioteca proxy.
2. El proxy construye una petición y la envía mediante una cola de mensajes POSIX.
3. El servidor recibe la petición y ejecuta la operación correspondiente.
4. El resultado se devuelve al cliente mediante una cola de respuesta privada.
5. El cliente recibe la respuesta y muestra el resultado de la operación.

## Concurrencia

El sistema permite la ejecución simultánea de múltiples clientes utilizando:

* Colas de mensajes POSIX.
* Sincronización mediante hilos.
* Procesamiento concurrente de peticiones.

Los programas auxiliares incluidos permiten realizar pruebas de concurrencia y rendimiento sobre el servicio distribuido.

## Resultados

La práctica implementa un sistema distribuido funcional capaz de gestionar operaciones clave-valor mediante comunicación cliente-servidor, bibliotecas dinámicas y colas de mensajes POSIX, garantizando el acceso concurrente de múltiples clientes.
