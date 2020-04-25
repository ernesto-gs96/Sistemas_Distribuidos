#include "SocketDatagrama.h"
#include "Respuesta.h"
#include<iostream>
#include<string.h>
using namespace std;

PaqueteDatagrama dp(sizeof(mensaje));


Respuesta::Respuesta(int puerto) {
	socketlocal = new SocketDatagrama(puerto);
	numSol = 0;
}
struct mensaje* Respuesta::getRequest() {
    // Creamos un datagrama de 'recibo'
	socketlocal->recibe(&dp);
    //Pasamos los datos    
	this->contenido = (struct mensaje*) dp.obtieneDatos();
	this->contenido->puerto = dp.obtienePuerto();
	memcpy(this->contenido->ip,dp.obtieneDireccion(),16);
	unsigned int reqId = 0;
	memcpy(&reqId, &this->contenido->requestId, sizeof(this->contenido->requestId));
	if(reqId < numSol){
		cout << "Paquete repetido..." << endl;
		return NULL;
	}
	else {
		numSol++;
		return this->contenido;
	}
	
}

void Respuesta::sendReply(char * respuesta, char * ipCliente, int puertoCliente) {
	struct mensaje *m1;
	m1 = (struct mensaje *) respuesta;
	cout << "Ip Cliente >: " << ipCliente << endl;
	PaqueteDatagrama datagramaEnvio((char*) m1, 100, ipCliente, puertoCliente);
	socketlocal->envia(&datagramaEnvio);
}
