#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream> 
#include <iterator> 
#include <fstream>
#include <fcntl.h>

#define TAM_MAX 34

char buffer[TAM_MAX];

using namespace std;

int main(int argc, char *argv[]){

	if(argc < 4){
		cout<<"MODO DE USO :"<<endl<<"./cliente <ip> <port> <archivo_origen>"<<endl;
		return -1;

	}
	
	char* ip = argv[1];
	int port = (int)*argv[2];
	int origen, nbytes, i = 0;

	if((origen = open(argv[3],O_RDONLY)) == -1){
        perror(argv[1]);
        exit(-1);
    }

	SocketDatagrama socket = SocketDatagrama(7000);

	while ((nbytes = read(origen,buffer,sizeof buffer)) > 0){

		//write(destino, buffer, nbytes);
		//Creamos un paquete datagram de 'envio'
		PaqueteDatagrama datagrama = PaqueteDatagrama((char *) buffer, sizeof(buffer),ip, 7001);
		socket.envia(&datagrama); //Se manda el datagrama al servidor
		cout << "i:" << i << endl;
		//Se crea un paquete datagram de 'recibo'
		//PaqueteDatagrama databack =  PaqueteDatagrama(sizeof(int));
		//socket.recibe(&databack); //Se recibe el datagrama de recibo

		//int * ans = (int*)databack.obtieneDatos(); //Obtenemso lo datos del datagrama de recibo
		cout << "Dato:" << datagrama.obtieneDatos() << endl;
		cout << " Dirección " << datagrama.obtieneDireccion() << " puerto: " << datagrama.obtienePuerto() << endl;
		//cout << "Respuesta="<<(*ans) << endl;
		i++;
	}
	
	return 0;
}