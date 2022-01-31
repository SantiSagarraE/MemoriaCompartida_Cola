# MemoriaCompartida_Cola
Script de linux. Este programa distingue dos procesos tipo A o B. Mediante cola de mensajes, el proceso A envia numeros pares que lee el proceso B y el proceso B envia numeros impares que lee el proceso A.

Se tiene un arreglo de 10 int, compartido entre dos procesos A y B. El proceso A
escribirá una secuencia de números pares en las primeras 5 posiciones del arreglo y
el proceso B escribirá una secuencia de números impares en las últimas 5
posiciones del arreglo.
Ambos procesos leen los valores escritos por el otro y los muestran en pantalla,
identificando la posición dentro del arreglo, identificándose como proceso A o B
junto con su PID, según corresponda.
La escritura en ambos casos se realiza a razón de un valor por segundo, mientras
que la lectura se realiza tan rápido como se pueda.
Cada proceso procede a escribir los cinco valores y, una vez concluida la
escritura, comenzará la lectura.
Un proceso no puede leer mientras el otro está escribiendo, y tampoco puede
escribir mientras el otro está leyendo.
El proceso que comience primero, escribirá los cinco valores y luego aguardará
hasta que el otro proceso haya escrito los suyos antes de comenzar a leerlos.
Este procedimiento se repetirá hasta que el Proceso A reciba la señal SIGTERM, y
cuando la reciba debe a su vez hacer terminar el proceso B, y liberar todos los
recursos compartidos utilizados.
Programa realiza con COLA DE MENSAJES.
