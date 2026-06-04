#!/bin/bash

# debemos haber iniciado la ejecucion del servidor antes de correr esto

echo "Prueba 23: Lanzando 20 clientes simultaneos..."
for i in {1..10}
do
   ./app_cliente_23_lento & 
   ./app_cliente_23_rapido & 
done

wait
echo "Prueba 23 terminada. Ver los logs en el servidor."