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

int main(int argc, char* argv[]){

  if(argc < 2){
		cout<<"MODO DE USO :"<<endl<<"./cliente <port> <archivo_destino>"<<endl;
		return -1;

	}
	
	int port = (int)*argv[1];
	int nbytes, destino, i = 0;

  if ((destino = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT,0666)) == -1){
    perror(argv[2]);
    exit(-1);
  }

  char * datos;

  //Se crea un socket para recibir informacipn en el puerto 7001
  SocketDatagrama socket = SocketDatagrama(7001);

	while(1){
    //Se crea un datagrama de 'recibo'
		PaqueteDatagrama datagrama = PaqueteDatagrama(sizeof(buffer));
		socket.recibe(&datagrama); //Se recibe el datagrama

    //Obtenemos los datos de datagrama y los copiamos
    memcpy(buffer, datagrama.obtieneDatos(), sizeof(datagrama.obtieneLongitud()));
    write(destino, buffer, sizeof(buffer));
		cout << "Dato:" << datagrama.obtieneDatos() << endl;
    //Del dataframa recibido obtenemos, datos, puerto y direccion IP
    cout << " Dirección " << datagrama.obtieneDireccion() <<" "<< "Puerto: " << datagrama.obtienePuerto() << endl;
    //Creamos un datagrama de 'envio' con la respuesta, el tamaño de la respuesta, dir IP y puerto de quien envia
    //PaqueteDatagrama databack =  PaqueteDatagrama((char*) &res, sizeof(bool), datagrama.obtieneDireccion(), datagrama.obtienePuerto());
	  //socket.envia(&databack); //Enviamos el paquete de envio
    }

    return 0;
}