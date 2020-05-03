#include "SocketDatagrama.h"
#include "Respuesta.h"
#include "Registro.h"
#include<iostream>
#include<string.h>
using namespace std;

PaqueteDatagrama dp(sizeof(mensaje));

Respuesta::Respuesta(int puerto) {
	socketlocal = new SocketDatagrama(puerto);
	numSol = 0;
}

struct mensaje* Respuesta::getRequest() {
	//RESETEO DE IDENTIFICADOR DE OPERACION
	if (numSol == 32767)
		numSol = 0;
    // Creamos un datagrama de 'recibo'
	socketlocal->recibe(&dp);
    //Pasamos los datos    
	contenido = (struct mensaje*) dp.obtieneDatos();
	contenido->puerto = dp.obtienePuerto();
	memcpy(contenido->ip,dp.obtieneDireccion(),16);
	//cout << contenido->ip << endl;
	unsigned int reqId = 0;
	memcpy(&reqId, &contenido->requestId, sizeof(contenido->requestId));
	//ESTA LOGICA NO SERVIRA PARA MULTICAST
	if(reqId < numSol){
		cout << "Paquete repetido..." << endl;
		return NULL;
	}
	else {
		numSol++;
		return contenido;
	}
	
}

void Respuesta::sendReply(char * respuesta, char * ipCliente, int puertoCliente) {
	struct mensaje *m1;
	m1 = (struct mensaje *) respuesta;
	cout << "Ip Cliente >: " << ipCliente << endl;
	PaqueteDatagrama datagramaEnvio((char*) m1, sizeof(m1), ipCliente, puertoCliente);
	socketlocal->envia(&datagramaEnvio);
}
