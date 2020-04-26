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
  int a, b , p = atoi(argv[1]);
  Respuesta respuesta(p);

  while (1)
  {
    struct mensaje mssgRecibido;
    struct mensaje messgEnviar;
    memcpy(&mssgRecibido, respuesta.getRequest(), sizeof(struct mensaje));

    int a = (mssgRecibido.arguments[0]);
    int b = (mssgRecibido.arguments[1]);
    int suma = a + b;

    cout << "ID :> " << mssgRecibido.operationId << endl;
    cout << "IP >: " << mssgRecibido.ip << endl;
    cout << "Puerto >: "<<mssgRecibido.puerto <<endl;

    if (mssgRecibido.operationId == 1)
    {
      cout << "La suma es >: " << suma << endl;
      memcpy(messgEnviar.arguments, (char *)&suma, sizeof(suma));
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