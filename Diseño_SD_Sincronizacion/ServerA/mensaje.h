#define TAM_MAX_DATA 65000
//Definicion de identificadores para operaciones permitidas
#define suma 1
// Definicion del formato de mensaje
struct mensaje{
	int messageType; //0= Solicitud, 1 = Respuesta 2 = Solicitud de servidor
	unsigned int requestId; //Identificador del mensaje
	int operationId; //Identificador de la operación
	char arguments[TAM_MAX_DATA];
};