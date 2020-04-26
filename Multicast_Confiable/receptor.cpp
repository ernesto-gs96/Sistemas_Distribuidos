#include "SocketMulticast.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TAM_MAX_DATA 650
using namespace std;

int main(int argc, char const *argv[])
{
    int nbd = 0;
    int monto;
    char* ip = (char *) argv[1];
    int puerto = atoi(argv[2]);

    SocketMulticast socketMulticast(puerto);
    PaqueteDatagrama paqueteDatagrama_Recepcion(sizeof(int));
    socketMulticast.unirseGrupo(ip);     

    while ( 1 ) {
        socketMulticast.recibeConfiable(paqueteDatagrama_Recepcion);
        memcpy(&monto,paqueteDatagrama_Recepcion.obtieneDatos(),sizeof(int));
        nbd = monto + nbd;
        cout<< "NBD: "<<nbd << endl;
    }

    socketMulticast.salirseGrupo(ip);
    return 0;
}