#include "SocketMulticast.h" 
#include "SocketDatagrama.h"
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

SocketMulticast::SocketMulticast(int puerto) {
    s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) {
        printf("Error al llamar a la función setsockopt\n");
        exit(0);
    }
    bzero((char *)&direccionLocal, sizeof(direccionLocal));
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY;
    direccionLocal.sin_port = htons(puerto);
    bind(s, (struct sockaddr *)&direccionLocal, sizeof(direccionLocal));
}

SocketMulticast::~SocketMulticast() {
    close(s);
}

void SocketMulticast::unirseGrupo(char* ip) {
    multicast.imr_multiaddr.s_addr = inet_addr(ip);
    multicast.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicast, sizeof(multicast));
}

void SocketMulticast::salirseGrupo(char *ip) {
    if ( multicast.imr_multiaddr.s_addr == inet_addr(ip) ) {
        setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void *) &multicast, sizeof(multicast));       
    }
}

int SocketMulticast::envia(PaqueteDatagrama &paqueteDatagrama, unsigned char ttl) {
    
    int n = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl));

    if ( n < 0 ) {
         printf("Ha ocurrido un error al enviar el paquete \n");
    } else {
        int client = sizeof(direccionForanea);
        bzero((char *)&direccionForanea, sizeof(direccionForanea));
        direccionForanea.sin_family = AF_INET;
        direccionForanea.sin_addr.s_addr = inet_addr(paqueteDatagrama.obtieneDireccion());
        direccionForanea.sin_port = htons(paqueteDatagrama.obtienePuerto());
        sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
    }
    
}

int SocketMulticast::enviaConfiable(PaqueteDatagrama & paqueteDatagrama, unsigned char ttl, int num_receptores) {
    
    int contador = 0, id,n,nn = 1;

    n = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl));
    while (n == -1)
    {
        cout << "Error en setsockopt del metodo enviaConfiable" << endl;
        n = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl));
    } 

    int client = sizeof(direccionForanea);
    bzero((char *)&direccionForanea, sizeof(direccionForanea));
    direccionForanea.sin_family = AF_INET;
    direccionForanea.sin_addr.s_addr = inet_addr(paqueteDatagrama.obtieneDireccion());
    direccionForanea.sin_port = htons(paqueteDatagrama.obtienePuerto());
    n = sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
    
    while (n == -1)
    {
        cout << "Error en sendto del metodo enviaConfiable" << endl;
        n = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl));
    } 
    SocketDatagrama socketUnicast(7200);

    for(int i = 0; i < num_receptores; i++ ){
        
        PaqueteDatagrama confirmacion(sizeof(int));

        n = socketUnicast.recibeTimeout(confirmacion,2,500000);
        while(n==-1)
        {
            if (nn == 7)
            {
                n = sendto(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t)client);
                if(n ==-1)
                    cout << strerror (errno) << endl;
                nn = 0;
            }
            
            cout << "Esperando respuesta:" << i << ", Volver a intentar:" << nn << endl;
            n = socketUnicast.recibeTimeout(confirmacion,2,500000);
            nn++;
        }
        
        memcpy(&id,confirmacion.obtieneDatos(),sizeof(int));

        contador++;
    }

    socketUnicast.~SocketDatagrama();

    if (num_receptores == contador) 
        return 1;
    else {
        cout << "FALTARON RESPUESTAS" << endl; 
        return -1;
    }

}

int SocketMulticast::recibe(PaqueteDatagrama &paqueteDatagrama) {
    int client = sizeof(direccionForanea);
    int n = recvfrom(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    paqueteDatagrama.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    paqueteDatagrama.inicializaPuerto(ntohs(direccionForanea.sin_port));
    
    return n;
}

int SocketMulticast::recibeConfiable(PaqueteDatagrama &paqueteDatagrama) {
    int aux;
    int client = sizeof(direccionForanea);
    int n = recvfrom(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    
    while (n == -1)
    {
        cout << "Error en recvfrom del metodo recibeConfiable" << endl;
        n = recvfrom(s, paqueteDatagrama.obtieneDatos(), paqueteDatagrama.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, (socklen_t*)&client);
    }

    paqueteDatagrama.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    paqueteDatagrama.inicializaPuerto(ntohs(direccionForanea.sin_port));
    std::cout << paqueteDatagrama.obtieneDireccion() << std::endl;
    
    SocketDatagrama socketUnicast(6666);  
    PaqueteDatagrama confirmacion(paqueteDatagrama.obtieneDatos(), sizeof(int),paqueteDatagrama.obtieneDireccion(),7200);
    n = socketUnicast.envia(confirmacion);
    while (n == -1)
    {
        cout << "ERROR ENVIA recibeConfiable" << endl;
        n = socketUnicast.envia(confirmacion);
    }
    
    socketUnicast.~SocketDatagrama();

    memcpy(&aux,paqueteDatagrama.obtieneDatos(),sizeof(int));

    return aux;
}
