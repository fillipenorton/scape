#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */ 

#define REMOTE_SERVER_PORT 50000
#define MAX_MSG 1024
	void inicializa(char * porta_local);
	void conectar(char * ip);
	void enviar_comando(char * com_text);


  int sd, rc, i,n,serLen;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;
  char msg[MAX_MSG];
	char comando[100];
  char ip_servidor[100];
  char porta[100];
  
  

int main(int argc, char *argv[]) {
  


  /* check command line args */
	if(argc!=3) {
		printf("usage : %s <servidor> <porta> \n", argv[0]);
		exit(1);
	}

	strcpy(ip_servidor,(char *)argv[1]);
	strcpy(porta,(char *)argv[2]);

	inicializa(porta);
	conectar(ip_servidor);
	enviar_comando("entrar");
 

	while(1) {

		/* init buffer */
		memset(msg,0x0,MAX_MSG);


		/* receive message */
		serLen = sizeof(remoteServAddr);
		n = recvfrom(sd, msg, MAX_MSG, 0, 
			(struct sockaddr *) &remoteServAddr, &serLen);

		

		if(n<0) {
			printf("cannot receive data \n");
			continue;
		}

		/* print received message */
		//printf("RETORNO DO SERVER %s \n",msg);

		if(strcmp(msg,"********************************************\n* Bem vindo ao mensageiro instantaneo Scape\n********************************************")==0){
			printf("%s \n",msg);
			scanf("%s",comando);
			enviar_comando(comando);	
		}
		else if(strcmp(msg,"Operacoes aceitas:\nLOGIN usuario senha\nADDUSER usuario senha\nSHOW_ONLINE\nMSG usuario mensagem\nLOGOUT\nEXIT")==0){
			printf("%s \n",msg);
			scanf("%s",comando);
			enviar_comando(comando);	
		}
		else if(strcmp(msg,"EXIT")==0){
			exit(1);
			scanf("%s",comando);
			enviar_comando(comando);	
		}
		else if(strcmp(msg,"esperando")==0){
			printf("Tente novamente:\n");
			scanf("%s",comando);
			enviar_comando(comando);	
		}
		else if(strcmp(msg,"Ainda falta implementar")==0){
			printf("Tente novamente: \n");
			scanf("%s",comando);
			enviar_comando(comando);	
		}else if(strcmp(msg,"usuario")==0){
			scanf("%s",comando);
			enviar_comando(comando);
			
		}else if(strcmp(msg,"senha")==0){
			scanf("%s",comando);
			enviar_comando(comando);
			
		}else if(strcmp(msg,"ADD")==0){
			printf("USUARIO CADASTRADO.\n");
			scanf("%s",comando);
			enviar_comando(comando);	
		}else if(strcmp(msg,"logout")==0){
			printf("Desconectado.\n");
			scanf("%s",comando);
			enviar_comando(comando);
		}else if(strcmp(msg,"login")==0){
			printf("Conectado.\n");
			scanf("%s",comando);
			enviar_comando(comando);
		}else{
			printf("%s",msg);
			scanf("%s",comando);
			enviar_comando(comando);		
		}
		
	}
  
  return 1;

}

void conectar(char * ip){
	h = gethostbyname(ip);

	if(h==NULL) {
		printf("host invalido '%s' \n", ip);
		exit(1);
	}

	//printf("Obtendo acesso em '%s' (IP : %s) \n", h->h_name,inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

	remoteServAddr.sin_family = h->h_addrtype;
	memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
	h->h_addr_list[0], h->h_length);
	remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);
}

void inicializa(char * porta_local){
	/* socket creation */
	sd = socket(AF_INET,SOCK_DGRAM,0);
	if(sd<0) {
		printf("cannot open socket \n");
		exit(1);
	}

	/* bind any port */
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliAddr.sin_port = htons(atoi(porta_local));

	rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
	if(rc<0) {
		printf("cannot bind port\n");
		exit(1);
	}

}

void enviar_comando(char * com_text){
	char comando[MAX_MSG];
	strcpy(comando,com_text);
    rc = sendto(sd, comando, strlen(comando)+1, 0, 
		(struct sockaddr *) &remoteServAddr, 
		sizeof(remoteServAddr));

    if(rc<0) {
      printf("cannot send data \n");
      close(sd);
      exit(1);
    }
}
