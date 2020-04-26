#include "Solicitud.h"
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char *argv[])
{

	if(argc < 5){
		cout<<"MODO DE USO :"<<endl<<"./cliente <ip> <puerto> <numero1> <numero2> <numeroSolicitudes>"<<endl;
		return -1;

	}
	//cout << "hola" << endl;
	char *response;
	char* ip = argv[1];
	int puerto = atoi(argv[2]);
	int numeros[2];
	int n = atoi(argv[5]);
	numeros[0] = atoi(argv[3]);
	numeros[1] = atoi(argv[4]);
	Solicitud cli;
	for(int i = 0; i <= n; i++){
		response = cli.doOperation(ip,puerto,1,(int*)numeros);
		cout<<response<<endl;
	}
	
	return 0;
}