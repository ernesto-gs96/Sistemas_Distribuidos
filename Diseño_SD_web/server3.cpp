#define MG_ENABLE_HTTP_STREAMING_MULTIPART 1
#include "mongoose.h"
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"
#include <stdio.h>
#include <iostream>
#include <sys/time.h>

using namespace std;

static const char *s_http_port = "8000"; //PUERTO
static struct mg_serve_http_opts s_http_server_opts;
struct timeval t1, t2; //PARAMENTROS PARA MEDIR EL TIEMPO DE RESPUESTA

static void handle_size(struct mg_connection *nc, struct http_message *hm, SocketDatagrama &soc) {
	
	char query[256];
	int tim;

	mg_get_http_var(&hm->body, "query", query,sizeof(query));
	gettimeofday(&t1, NULL);
	PaqueteDatagrama paq(100);
	for (int i = 0; i < 7; i++){
		int n = soc.recibeTimeout(&paq,2,500000);
		//int n = soc.recibeTimeout(paq,2,500000);

		if(n >= 0){
			gettimeofday(&t2, NULL);
			sprintf(query, "IP: %s  , Tiempo: 2", paq.obtieneDireccion());
			printf("Cadena enviada: %s\n", query);
		}
	}

	mg_send_head(nc,200,strlen(query), "Content-Type: text/plain");
	mg_printf(nc, "%s", query);
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	
	char query[256];
 	struct http_message *hm = (struct http_message *) p;

	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/search") == 0) { 
			
			mg_get_http_var(&hm->body, "query", query,sizeof(query));
			printf("Cadena introducida: %s\n",query);
			PaqueteDatagrama paq(NULL, 0, query, 7777); //NO SE LE ENVIAN DATOS MAS QUE LA DIRECCION IP Y EL PUERTO
			SocketDatagrama soc(7777);
			soc.envia(&paq);
		    handle_size(nc, hm, soc);  
		}else{
			mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
		}
	}

}

int main(void) {

	struct mg_mgr mgr;
	struct mg_connection *nc;
	mg_mgr_init(&mgr, NULL);

	printf("Starting web server on port %s\n", s_http_port);
	nc = mg_bind(&mgr, s_http_port, ev_handler);

	if (nc == NULL) {
		printf("Failed to create listener\n");
		return 1;
	}
	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = "www"; // Serve current directory
	s_http_server_opts.enable_directory_listing = "yes";
	
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);

	return 0;
}
