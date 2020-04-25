#include "SocketMulticast.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TAM_MAX_DATA 650

int main(int argc, char const *argv[])
{
    if(argc <3){
        std::cout<<"FORMA DE USO >: ./receptor <ip> <puerto>"<<std::endl;
        return -1;
    }
    int numeros[2];
    char* ip = (char *) argv[1];
    int puerto = atoi(argv[2]);

    SocketMulticast socketMulticast(puerto);
    PaqueteDatagrama datagramaRecibo(2 * sizeof(int));

    socketMulticast.unirseGrupo(ip);     

    socketMulticast.recibe(datagramaRecibo);

    socketMulticast.salirseGrupo(ip);

    memcpy(&numeros,datagramaRecibo.obtieneDatos(),2 * sizeof(int));

    std::cout << "Cliente: \n" <<  "IP:" << datagramaRecibo.obtieneDireccion();
    std::cout << ", Puerto: " << datagramaRecibo.obtienePuerto() << "\n";
    int aux = numeros[0] + numeros[1];
    std::cout << "Numeros recibidos: a = " << numeros[0] <<  ", b = " << numeros[1] << std::endl;
    int* resp = &(aux);


    SocketDatagrama socketDatagrama(7200);
    PaqueteDatagrama paqueteDatagrama_Envio((char*) resp ,sizeof(int),datagramaRecibo.obtieneDireccion(),7200);
    socketDatagrama.envia(paqueteDatagrama_Envio);
    

    return 0;
}
