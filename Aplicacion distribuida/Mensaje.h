#ifndef __Mensaje__
#define __Mensaje__
#define TAM_MAX_DATA 4000
//Definicion de identificadores para operaciones permitidas

// Definicion del formato de mensaje
struct mensaje
{
    int puerto;
    char ip[16];
    int messageType;        //0 = Solicitud, 1 = Respuesta
    unsigned int requestId; //Identificador del mensaje
    int operationId;        //Identificador de la operación
    int arguments[TAM_MAX_DATA];
};

#endif