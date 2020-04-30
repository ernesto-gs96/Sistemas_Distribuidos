#include "Solicitud.h"
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <string.h>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{

	if(argc < 3){
		cout<<"MODO DE USO :"<<endl<<"./cliente <ip> <puerto> <numeroSolicitudes>"<<endl;
		return -1;

	}

	srand (time(NULL));
	char* response;
	char* ip = argv[1];
	int puerto = atoi(argv[2]), cantidad[1], n = atoi(argv[3]), ires, sumIdeal = 0;
	Solicitud cli;
	for(int i = 0; i <= n; i++){
		cantidad[0] = rand() % 9 + 1;		
		response = cli.doOperation(ip,puerto,1,(int*)cantidad);
		memcpy(&ires, response, 4);
		sumIdeal += cantidad[0];
		if(sumIdeal != ires){
			cout << "Error: Monto incorrecto, esperado: " << sumIdeal << ", obtenido: " << ires << endl;
			exit(-1);
		}else
			cout << "Esperado: " << sumIdeal << ", obtenido: " << ires << endl;
		cout <<"------------------------------------------------------------" << endl;
	}

	
	return 0;
}