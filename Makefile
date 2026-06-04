# Compilador
CC = gcc

# Opciones de compilacion: mostrar warnings y bibliotecas dinámicas
CFLAGS = -Wall -fPIC

# Opciones de enlazado: buscar librerías en el directorio actual y hacer que el ejecutable busque las .so en el directorio actual
LDFLAGS = -L. -Wl,-rpath=.

# Librerías de colas POSIX e hilos
LDLIBS = -lrt -lpthread


# PARTE A

# Compila la versión no distribuida
parteA: libclaves.so app_cliente_A

# Construcción de la biblioteca dinámica local
libclaves.so: claves.o
	$(CC) -shared -o $@ $^

# Cliente de la parte A enlazado con libclaves.so
app_cliente_A: app_cliente.o libclaves.so
	$(CC) $(LDFLAGS) app_cliente.o -lclaves -o $@


# PARTE B

# Lista de ejecutables de la parte B
BIN_FILES_B = servidor app_cliente_B app_cliente_23_lento app_cliente_23_rapido app_cliente_24 app_cliente_21_1 app_cliente_21_2

# Compila la versión distribuida
parteB: libclaves.so libproxyclaves.so $(BIN_FILES_B)

libproxyclaves.so: proxy-mq.o
	$(CC) -shared -o $@ $^ $(LDLIBS)

servidor: servidor-mq.o libclaves.so
	$(CC) $(LDFLAGS) servidor-mq.o -lclaves $(LDLIBS) -o $@

# Cliente principal de la parte B
app_cliente_B: app_cliente.o libproxyclaves.so
	$(CC) $(LDFLAGS) app_cliente.o -lproxyclaves $(LDLIBS) -o $@

# Cliente adicional 23 lento
app_cliente_23_lento: app_cliente_23_lento.o libproxyclaves.so
	$(CC) $(LDFLAGS) app_cliente_23_lento.o -lproxyclaves $(LDLIBS) -o $@

# Cliente adicional 23 rápido
app_cliente_23_rapido: app_cliente_23_rapido.o libproxyclaves.so
	$(CC) $(LDFLAGS) app_cliente_23_rapido.o -lproxyclaves $(LDLIBS) -o $@

# Cliente adicional 24
app_cliente_24: app_cliente_24.o libproxyclaves.so
	$(CC) $(LDFLAGS) app_cliente_24.o -lproxyclaves $(LDLIBS) -o $@

# Cliente adicional 21_1
app_cliente_21_1: app_cliente_21_1.o libproxyclaves.so
	$(CC) $(LDFLAGS) app_cliente_21_1.o -lproxyclaves $(LDLIBS) -o $@

# Cliente adicional 21_2
app_cliente_21_2: app_cliente_21_2.o libproxyclaves.so
	$(CC) $(LDFLAGS) app_cliente_21_2.o -lproxyclaves $(LDLIBS) -o $@

# Objeto de claves.c
claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

# Objeto del proxy
proxy-mq.o: proxy-mq.c mensaje.h claves.h
	$(CC) $(CFLAGS) -c proxy-mq.c

# Objeto del servidor
servidor-mq.o: servidor-mq.c mensaje.h claves.h
	$(CC) $(CFLAGS) -c servidor-mq.c

# Objeto del cliente principal
app_cliente.o: app_cliente.c claves.h
	$(CC) -Wall -c app_cliente.c

# Objetos de clientes adicionales
app_cliente_23_lento.o: app_cliente_23_lento.c claves.h
	$(CC) -Wall -c app_cliente_23_lento.c

app_cliente_23_rapido.o: app_cliente_23_rapido.c claves.h
	$(CC) -Wall -c app_cliente_23_rapido.c

app_cliente_24.o: app_cliente_24.c claves.h
	$(CC) -Wall -c app_cliente_24.c

app_cliente_21_1.o: app_cliente_21_1.c claves.h
	$(CC) -Wall -c app_cliente_21_1.c

app_cliente_21_2.o: app_cliente_21_2.c claves.h
	$(CC) -Wall -c app_cliente_21_2.c

# Limpia solo la parte A
cleanA:
	rm -f *.o libclaves.so app_cliente_A

# Limpia solo la parte B
cleanB:
	rm -f *.o libproxyclaves.so servidor app_cliente_B app_cliente_23_lento app_cliente_23_rapido app_cliente_24 app_cliente_21_1 app_cliente_21_2

# Limpia todo
clean:
	rm -f *.o *.so servidor app_cliente_A app_cliente_B app_cliente_23_lento app_cliente_23_rapido app_cliente_24 app_cliente_21_1 app_cliente_21_2