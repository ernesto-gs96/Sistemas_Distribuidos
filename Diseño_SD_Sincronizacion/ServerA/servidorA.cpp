#include "Respuesta.h"
#include "Solicitud.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

// Nodo 
struct TrieNode { 
    struct TrieNode *children[10]; 
    // isEndOfWord is true si el nodo representa
    // fin de cadena
    bool isEndOfWord; 
}; 
  
// Returna un nuevo nodo (initialized to NULLs) 
struct TrieNode *getNode(void) { 
    struct TrieNode *pNode =  new TrieNode; 
    pNode->isEndOfWord = false; 
  
    for (int i = 0; i < 9; i++) 
        pNode->children[i] = NULL; 
  
    return pNode; 
} 
  
// Si no existe, inserta key dentro de nodo 
// Si la key es prefijo del nodo, sólo
// marca al nodo 
void insert(struct TrieNode *root, string key) { 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) { 
        int index = key[i] - '0'; 
        if (!pCrawl->children[index]) 
            pCrawl->children[index] = getNode(); 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    // marcamos el ultimo nodo 
    pCrawl->isEndOfWord = true; 
} 
  
// Returna true si la key esta en el  trie, de lo contrario false
bool search(struct TrieNode *root, string key) { 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) { 
        int index = key[i] - '0'; 
        if (!pCrawl->children[index]) 
            return false; 
  
        pCrawl = pCrawl->children[index]; 
    }
    return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Forma de uso >: ./server puerto ip_servidorB puertoB ip_servidorC puertoC\n");
        exit(0);
    }
    int vector[4]= {0,0,0,0};
    Respuesta resp(atoi(argv[1]));

    char* s1Ip = argv[2];
	int s1Port = atoi(argv[3]);

	char* s2Ip = argv[4];
	int s2Port = atoi(argv[5]);

    struct mensaje sms;
    Solicitud s;

    struct TrieNode *root = getNode(); 
    cout << "Servidor iniciado A....\n";
    std::ofstream file1("registrosA.txt", std::ofstream::out | std::ofstream::trunc);
    file1.close();
    ofstream output;

    while (true) {
        struct mensaje *msj = resp.getRequest();
        if(msj != NULL) {
            output.open("registrosA.txt", ios::out | ios::app );
            struct registro r;
            memcpy(&r, &msj->arguments, sizeof(registro));

            struct timeval actual;
            actual.tv_sec = 0;
            actual.tv_usec = 0;

            if(!search(root, string(r.celular))){
                insert(root, string(r.celular));
                gettimeofday(&actual,NULL);
                string regs = r.toString();

                cout << actual.tv_sec << endl;
                cout << actual.tv_usec << endl;

                regs += to_string(actual.tv_sec) + to_string(actual.tv_usec);
                output.write(regs.c_str(),regs.length());
                output.write("\n",1);
                output.flush();
                resp.sendReply((char *)&actual);
                vector[3] = atoi((char * )&actual);
                vector[0] = vector[0] + 1; //Se incrementa una vez confirmado su almacenamiento y su enviado su confirmación
                s.doOperation(s1Ip, s1Port, 1, (char *)&vector);
                s.doOperation(s2Ip, s2Port, 1, (char *)&vector);
               
            }else{
            	cout << "Telefono ya registrado" << endl;
                resp.sendReply((char *)&actual);
            }
            output.close();
        }
        cout << endl;
    }
}