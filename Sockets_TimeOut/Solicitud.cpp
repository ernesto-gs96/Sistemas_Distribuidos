#include "SocketDatagrama.h"
#include "Solicitud.h"
#include "Mensaje.h"
#include <string.h>

Solicitud::Solicitud() {
	socketlocal = new SocketDatagrama(0);
}

char * Solicitud::doOperation(char* ip, int puerto, int operationId, int* arguments) {
	struct mensaje msj;
	
	int cont = 0;
	int bytes = -1;

	msj.messageType = 0;
	msj.requestId = id;
	id++;
	memcpy(msj.ip, ip, 16);
	msj.puerto = puerto;
	msj.operationId = operationId;

	cout << "Id operacion: " << msj.operationId << endl;

	cout << "ip: " << msj.ip << endl;
	memcpy(msj.arguments, arguments, 100);
	cout << "puerto: " << msj.puerto << endl;
	cout << "argumentos: " << msj.arguments[1] << endl;

	PaqueteDatagrama paq((char*) &msj, sizeof(msj), ip, puerto);
	

	//SocketDatagrama socket(puerto);//
	//PaqueteDatagrama paq((char*) &msj, sizeof(msj), ip, puerto);
	//Utilizar recibetimeot para que reenvie la misma solicitud hasta 7 veces, si no se logra
	//termina e imprime servidor no disponible 
	while(1){
	while (cont < 7 && bytes == -1) 
	{
		socketlocal -> envia(&paq);
		PaqueteDatagrama paq1(sizeof(msj));
		//int recibeTimeout(PaqueteDatagrama *p, time_t segundos, suseconds_t microsegundos);
		bytes = socketlocal -> recibeTimeout(&paq1, 2, 500000);
		//socketlocal -> recibeTimeout(&paq1, 2, 500000);
		
		
		//cout << socketlocal -> recibeTimeout(&paq1,4, 500000) << endl; //-1
		//socketlocal -> recibe(&paq1);		

		if(bytes > 1)
		{
		//	cout << "Bytes: " << bytes << endl;
			char* resultado;
			resultado = paq1.obtieneDatos();
			int r;
			memcpy(&r,resultado,sizeof(resultado));
			cout << "Suma en el servidor -> " << r << endl;
			return resultado;
		}
			
		else
		{
			cont ++;
		}

		if(cont == 7)
		{
		cout << "Servidor no disponible, intenta mas tarde" << endl;
		exit(1);
		}
		
	}

	
	}
		

	
}
