#include "SocketMulticast.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


#define TAM_MAX_DATA 650
using namespace std;

int main(int argc, char const *argv[]){

    if (argc < 4){
        cout<<"MODO DE USO >: ./emisor <ip> <puerto> <ttl>"<<endl;
        return -1;
    }

    char* ip = (char*) argv[1];
    int puerto = atoi(argv[2]);
    int t = atoi(argv[3]);
    unsigned char ttl = (unsigned char) t; //Tiempo de vida (número de routers permitido)

    int numeros[2];
    int resp;
    int intentos = 7,n;

    cout<<"Ingrese el primer numero :> ";cin>>numeros[0];
    cout<<"Ingrese el segundo numero :> ";cin>>numeros[1];
    cout<<endl<<endl;
    SocketMulticast socketMulticast(puerto);
    PaqueteDatagrama datagramaEnvio((char*) numeros,2 * sizeof(int),ip,puerto);
    
    socketMulticast.envia(datagramaEnvio,ttl);

    SocketDatagrama socketDatagrama(7200);
    PaqueteDatagrama datagramaRecibo(sizeof(int));

    do {
        n = socketDatagrama.recibeTimeout(datagramaRecibo,2,600000);
        if (n == -1){
            intentos--;
        } else {
            memcpy(&resp,datagramaRecibo.obtieneDatos(),sizeof(int));            
            cout<<"***************Información recibida******************"<<endl;
            cout << "IP: " << datagramaRecibo.obtieneDireccion() << endl;            
            cout << "Suma :" << resp << "\n" << endl;
            cout<<"*****************************************************"<<endl;
            cout<<endl;
        }
    }while((intentos > 0));

    if (n == -1) {

         cout << "Tiempo de respuesta agotado" << endl;
         exit(0);

    } 


    return 0;
}
