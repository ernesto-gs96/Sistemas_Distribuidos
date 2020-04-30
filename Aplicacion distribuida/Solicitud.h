#ifndef __Solicitud__
#define __Solicitud__

#include "SocketDatagrama.h"
#include <iostream>

using namespace std;

class Solicitud {
    
public:
    Solicitud();
    char * doOperation(char *IP, int puerto, int operationId, int *arguments);
    
private:
    SocketDatagrama *socketlocal;
};

#endif