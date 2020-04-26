#include "SocketDatagrama.h"
#include "Respuesta.h"
#include<iostream>
#include<string.h>
using namespace std;


Respuesta::Respuesta(int puerto) {
	socketlocal = new SocketDatagrama(puerto);

}
struct mensaje* Respuesta::getRequest() {
    // Creamos un datagrama de 'recibo'
	PaqueteDatagrama datagramaRecibo(sizeof(mensaje));
	socketlocal->recibe(&datagramaRecibo);
    //Pasamos los datos    
	this->contenido = (struct mensaje*) datagramaRecibo.obtieneDatos();
	this->contenido->puerto = datagramaRecibo.obtienePuerto();
	memcpy(this->contenido->ip,datagramaRecibo.obtieneDireccion(),16);
	return this->contenido;
}

void Respuesta::sendReply(char * respuesta, char * ipCliente, int puertoCliente) {
	struct mensaje *m1;
	m1 = (struct mensaje *) respuesta;
	cout << "Ip Cliente >: " << ipCliente << endl;
	cout << "Puerto Cliente >: " << puertoCliente << endl;
	PaqueteDatagrama datagramaEnvio((char*) m1, 100, ipCliente, puertoCliente);
	socketlocal->envia(&datagramaEnvio);
}
