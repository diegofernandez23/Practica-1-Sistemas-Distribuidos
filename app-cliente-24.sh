#!/bin/bash

# debemos haber iniciado la ejecucion del servidor antes de correr esto

echo "Prueba 24: Lanzando 42 clientes de colision simultaneos..."
for i in {1..42}
do
   ./app_cliente_24
done

wait
echo "Prueba 24 terminada. Ver los logs en el servidor."