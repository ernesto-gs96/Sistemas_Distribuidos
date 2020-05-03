#include "Solicitud.h"
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include "Registro.h"
#include "Mensaje.h"
#include <cstdlib>
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

    struct mensaje m1, m11;
    char * m2;
    cout << "PRUEBAS" << endl;
    memcpy(m1.registro.celular, "951", sizeof(m1.registro.celular));
    m1.puerto = 52;

    memcpy(&m11,&m1,sizeof(m1));

    cout << m11.puerto << endl;
    cout << m11.registro.celular << endl;
    
    //cout << m11.puerto << endl;
    //m2 = (char*)malloc(sizeof(m1) * sizeof(char));
    //memcpy( m2, (char*)&m1, sizeof(m1));

    //struct mensaje *m3;
    //m3 = (struct mensaje *)malloc(sizeof(m1));
    //m3 = (struct mensaje *)m2;
    //cout << m3->puerto << endl;
    //cout << m3->registro->celular << endl;

    //struct mensaje m4;
    //char *m5;
    //m5 = (char*)malloc(sizeof(m4) * sizeof(char));
    //memcpy( m5, (char*)&m1, sizeof(m1));

    //m4 = &m3;
    //memcpy(&m4, m3, sizeof(m4));
    //int puerto = 
    //cout << m4.puerto << endl;
    //cout << m4.registro->celular << endl;
	
    //free(m2);
	return 0;
}