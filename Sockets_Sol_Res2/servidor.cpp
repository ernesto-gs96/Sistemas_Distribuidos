#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <vector>
#include "Respuesta.h"
using namespace std;

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    cout << "Modo de uso" << endl
         << "./Servidor <puerto>" << endl;
    return -1;
  }
  int p = atoi(argv[1]);
  int nbd = 0;  
  Respuesta respuesta(p);

  while (1)
  {
    struct mensaje mssgRecibido;
    struct mensaje messgEnviar;
    memcpy(&mssgRecibido, respuesta.getRequest(), sizeof(struct mensaje));

    int deposito = (mssgRecibido.arguments[0]);
    nbd += deposito;

    cout << "ID :> " << mssgRecibido.requestId << endl;

    if (mssgRecibido.operationId == 1)
    {
      cout << "NBD >: " << nbd << endl;
      memcpy(messgEnviar.arguments, (char *)&nbd, sizeof(nbd));
      messgEnviar.messageType = 1;
      memcpy(messgEnviar.ip, mssgRecibido.ip, 16);
      messgEnviar.puerto = mssgRecibido.puerto;
      messgEnviar.requestId = mssgRecibido.requestId;
      respuesta.sendReply((char *)messgEnviar.arguments, messgEnviar.ip, mssgRecibido.puerto);
      cout << "-----------------------------------------" << endl;
    }
  }
  return 0;
}