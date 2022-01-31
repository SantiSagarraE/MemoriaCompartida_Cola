/*
Este programa distingue dos procesos tipo A o B. Mediante
cola de mensajes, el proceso A envia numeros pares que lee el
proceso B y el proceso B envia numeros impares que lee el 
proceso A. El programa finaliza enviando la señal SIGTERM a cualquiera
de los dos procesos. 
*/

#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef struct{
	long Id_Mensaje;
	int Dato_Numerico;
}Mi_Tipo_Mensaje;

int salir=0;

void trapper1(int sig); 

int main(void){
	key_t Clave1;
	int Id_Cola_Mensajes, j, i=0, pidB=0, pidA=0, flagA=0, flagB=0;
	Mi_Tipo_Mensaje Un_Mensaje;
	char nombre[10];

	signal(SIGTERM, trapper1);

	/* Declaracion de la cola de mensajes */
	Clave1 = ftok ("/bin/ls", 33);
	if (Clave1 == (key_t)-1){
		printf("Error al obtener clave para cola mensajes" "\n");
		exit(-1);
	}
	Id_Cola_Mensajes = msgget (Clave1, 0600 | IPC_CREAT);
	if (Id_Cola_Mensajes == -1){
		printf("Error al obtener identificador para cola mensajes" "\n");
		exit (-1);
	}

	/* Inicio del programa */
	printf("Identifique el proceso a utilizar \n");		// Identifico que proceso quiero activar
	scanf("%[^\n]" ,nombre);
	if(strcmp(nombre, "proceso a") == 0){
		printf("Soy el proceso A de pid %d\n",getpid());
		flagA=1;
	}else if(strcmp(nombre, "proceso b") == 0){
		printf("Soy el proceso B de pid %d\n",getpid());
		flagB=1;
	}else{
		printf("No se indentifico el proceso\n");
		exit(-1);
	}
	
	while(pidA==0){									// De esta manera el proceso B obtiene la PID
		if(flagA){									// del proceso A para luego poder terminarlo
			Un_Mensaje.Id_Mensaje = 1;
			Un_Mensaje.Dato_Numerico =getpid();
			pidA = getpid();
			msgsnd (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico), 0);
		}
		if(flagB){
			msgrcv (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje,sizeof(Un_Mensaje.Dato_Numerico), 1, 0);
			pidA=Un_Mensaje.Dato_Numerico;
		}
	}

	while(pidB==0){									// El proceso A obtiene la PID del proceso B 
		if(flagB){								
			Un_Mensaje.Id_Mensaje = 2;
			Un_Mensaje.Dato_Numerico =getpid();
			pidB = getpid();
			msgsnd (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico), 0);
		}
		if(flagA){
			msgrcv (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje,sizeof(Un_Mensaje.Dato_Numerico), 2, 0);
			pidB=Un_Mensaje.Dato_Numerico;
		}
	}

	
	
	while(1){
		if(flagA){
			Un_Mensaje.Id_Mensaje = 1;			// Distingo dos tipos de mensajes para proceso A o B
			for (j=0; j<=4; j++){				// Mensaje tipo 1, envio numeros pares a la cola	
				Un_Mensaje.Dato_Numerico =(rand()%500)*2;
				i++;
				msgsnd (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico), 0);
				printf(".");
				sleep(1);
			}
			printf("\n");
			for (j=0; j<=4; j++){				// Mensaje tipo 2, recibo los numeros escritos por el proceso B
			msgrcv (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje,sizeof(Un_Mensaje.Dato_Numerico), 2, 0);
			printf("Soy el proceso A, PID= %d :Memoria[%d] = %d \n", getpid(), j+5, Un_Mensaje.Dato_Numerico);
			}
		}	
		if(flagB){
			Un_Mensaje.Id_Mensaje = 2;			// Mensaje tipo 2 para el proceso B
			for (j=0; j<=4; j++){	
				Un_Mensaje.Dato_Numerico =(rand()%500)*2+1;
				i++;
				msgsnd (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje, sizeof(Un_Mensaje.Dato_Numerico), 0);
				printf(".");
				sleep(1);
			}
			printf("\n");
			for (j=0; j<=4; j++){				// Recibo los mensajes tipo 1 
			msgrcv (Id_Cola_Mensajes, (struct msgbuf *)&Un_Mensaje,sizeof(Un_Mensaje.Dato_Numerico), 1, 0);
			printf("Soy el proceso B, PID= %d :Memoria[%d] = %d \n", getpid(), j, Un_Mensaje.Dato_Numerico);
			}
		}
		if(salir==1 && flagA){				// Rutina para matar proceso B y terminar con el propio proceso
			printf("Soy el proceso A y procedo a matar al proceso B en caso de que este activo\n");
			kill(pidB,SIGTERM);
			printf("Soy el proceso A y voy a morir\n");	
			msgctl (Id_Cola_Mensajes, IPC_RMID, (struct msqid_ds *)NULL);
			return 0;
		}	
		if(salir==1 && flagB){				// Rutina para matar proceso A y terminar con el propio proceso
			printf("Soy el proceso B y procedo a matar al proceso A en caso de que este activo\n");
			kill(pidA,SIGTERM);
			printf("Soy el proceso B y voy a morir\n");	
			msgctl (Id_Cola_Mensajes, IPC_RMID, (struct msqid_ds *)NULL);
			return 0;
		}
	}	
}


void trapper1(int sig){
	salir=1;	 		
}