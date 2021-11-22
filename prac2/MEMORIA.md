# SISTEMAS OPERATIVOS - CONCURRENCIA Y ENVIO DE MENSAJES
## 2021 - 2022

Manuel Morales Amat

---

Para el servicio de cliente he implementado un código que abre un socket en la ip dada por parametro, en caso de no pasarle nada asigna localhost o 127.0.0.1 como ip predeterminada. Una vez abierto el canal descarga un archivo html que muestra en la terminal y abre en firefox.

Para el servicio servidor he implementado un código que busca si existe un archivo llamado 'Google.html', en caso de no existir hace un wget para descargar una copia exacta del código fuente visible de la web; y lo envia a través de un socket que se habre confirmando la conexión del cliente.

---

Para solventar el problema de concurrencia tomo 3 semáforos, 1 para el bufer, otro para el consumidor y otro para el productor.
Inicializo el consumidor a 0 para que sea el primero en ser bloqueado.
El productor produce quitando 1 heuco al bufer y desbloqueando al consumidor.
El consumidor consume dando 1 hueco al bufer y bloqueandose si no hay hueco en el bufer o el productor esta aún produciendo.
El apartado de graficos no lo he podido resolver.