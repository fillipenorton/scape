#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

#define LOCAL_SERVER_PORT 50000
#define MAX_MSG 1025

void inicializa();

void enviar_resposta(char * com_text,struct sockaddr_in destino);

struct ficha_usuario{
	char nome[20];
	int status;
	int id;
};

  int sd, rc, n, i, cliLen,temp,num_pessoa=0,num_logado=0,j;

  struct sockaddr_in cliAddr, servAddr,pessoa[40],/*logado[40],*/jog;

  char msg[MAX_MSG];
  char resposta[MAX_MSG];
  char senha[20],usuario[20],login[40],Slogin[40];
	FILE *mread;
	char *arquivo, *show_logados;
	char aux[9999];

int main(int argc, char *argv[]) {
struct ficha_usuario f_usuario[num_pessoa]; 
  //printf("%s: waiting for data on port UDP %u\n", argv[0],LOCAL_SERVER_PORT);
	if(argc!=2) {
		printf("usage : %s <ARQUIVO> \n", argv[0]);
		exit(1);
	} else
		arquivo = argv[1];
	
	inicializa();
	


  /* server infinite loop */
  while(1) {
    
    /* init buffer */
    memset(msg,0x0,MAX_MSG);


    /* receive message */
    cliLen = sizeof(cliAddr);
    n = recvfrom(sd, msg, MAX_MSG, 0, 
		 (struct sockaddr *) &cliAddr, &cliLen);

    if(n<0) {
      printf("%s: cannot receive data \n",argv[0]);
      continue;
    }
    
    
    /* Aqui começa o programa */  
    
    if(strcmp(msg,"entrar")==0){
		if(num_pessoa < 40){
			printf("SCAPE: nova conexao a partir de %s:%d \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
			//printf("");
			pessoa[num_pessoa] = cliAddr;
			f_usuario[num_pessoa].id = num_pessoa;
			f_usuario[num_pessoa].status = 0;
			sprintf(resposta,"********************************************\n* Bem vindo ao mensageiro instantaneo Scape\n********************************************");
			enviar_resposta(resposta,pessoa[num_pessoa]);		
			num_pessoa++;
		}
    }
    	
    else if(strcmp(msg,"HELP")==0){
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			printf("o");
    			sprintf(resposta,"Operacoes aceitas:\nLOGIN usuario senha\nADDUSER usuario senha\nSHOW_ONLINE\nMSG usuario mensagem\nLOGOUT\nEXIT");
    			enviar_resposta(resposta,pessoa[i]);
    		}
    	}
    }
    else if(strcmp(msg,"EXIT")==0){
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			sprintf(resposta,"EXIT");
    			enviar_resposta(resposta,pessoa[i]);
    			num_pessoa--;
    		}
    	}
    }
    else if(strcmp(msg,"ADDUSER")==0){
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			sprintf(resposta,"usuario");
    			enviar_resposta(resposta,pessoa[i]);
    			//memset(msg,0x0,MAX_MSG);
    			//cliLen = sizeof(cliAddr);
    			n = recvfrom(sd, msg, MAX_MSG, 0,(struct sockaddr *) &cliAddr, &cliLen);
				sprintf(usuario,msg);
				sprintf(resposta,"senha");
				enviar_resposta(resposta,pessoa[i]);
				n = recvfrom(sd, msg, MAX_MSG, 0,(struct sockaddr *) &cliAddr, &cliLen);
				sprintf(senha,msg);
				
				
				
				
				
				//FILE *mread;
				
				mread = fopen(arquivo,"a");
				if (mread == NULL){
					printf("falha");
					return 1;
				}
				else{
					fprintf(mread,"%s:%s\n",usuario,senha);
				}
				fclose(mread);
				sprintf(resposta,"ADD");
				enviar_resposta(resposta,pessoa[i]);
			}


		}
	}
    else if(strcmp(msg,"AUTH")==0){

		for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			sprintf(resposta,"usuario");
    			enviar_resposta(resposta,pessoa[i]);
    			//memset(msg,0x0,MAX_MSG);
    			//cliLen = sizeof(cliAddr);
    			n = recvfrom(sd, msg, MAX_MSG, 0,(struct sockaddr *) &cliAddr, &cliLen);
				sprintf(usuario,msg);
				sprintf(resposta,"senha");
				enviar_resposta(resposta,pessoa[i]);
				n = recvfrom(sd, msg, MAX_MSG, 0,(struct sockaddr *) &cliAddr, &cliLen);
				sprintf(senha,msg);
				
				sprintf(login,"%s:%s",usuario,senha);
				
				mread = fopen(arquivo, "r");
					if (mread == NULL) {
						printf("falha ao abrir o arquivo.\n");
					return 1;
				}
				while(!feof(mread)) {
					fscanf(mread,"%s",Slogin);
					if(strcmp(login,Slogin)==0){
							f_usuario[i].id = i;
							f_usuario[i].status = 1;
							sprintf(f_usuario[i].nome,usuario);
							sprintf(resposta,"login");
							enviar_resposta(resposta,pessoa[i]);
					}
				}
				fclose(mread);
				
				
				
    		}
    	}
		

    }
    
    
    /* precisa está logado */
    else if(strcmp(msg,"SHOW_ONLINE")==0){
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			for (j=0 ; j<num_pessoa ; j++){
					if (f_usuario[j].status == 1){
						strcat(aux,f_usuario[j].nome);
						strcat(aux,"\n");
						
						sprintf(resposta,aux);
						enviar_resposta(resposta,pessoa[i]);
					}
				}
    		}
    	}
    }
    else if(strcmp(msg,"MSG")==0){
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			
    			sprintf(resposta,"Ainda falta implementar");
    			enviar_resposta(resposta,pessoa[i]);
    		}
    	}
    }
    else if(strcmp(msg,"LOGOUT")==0){
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			f_usuario[i].status = 0; 
    			sprintf(resposta,"logout");
    			enviar_resposta(resposta,pessoa[i]);
    		}
    	}
    }
    else{
    	for (i=0;i<num_pessoa;i++){
    		if((cliAddr.sin_addr.s_addr == pessoa[i].sin_addr.s_addr) && (cliAddr.sin_port == pessoa[i].sin_port)){
    			enviar_resposta("esperando",pessoa[i]);
    		}
    	}
    }
    
    
  }/* end of server infinite loop */

return 0;

}

void inicializa(){
	/* socket creation */
	sd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sd<0) {
		printf("cannot open socket \n");
		exit(1);
	}

	/* bind local server port */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));

	if(rc<0) {
		printf("cannot bind port");
		exit(1);
	}

}

void enviar_resposta(char * com_text,struct sockaddr_in destino) {
	char comando[MAX_MSG];
	strcpy(comando,com_text);
    rc = sendto(sd, comando, strlen(comando)+1, 0, 
		(struct sockaddr *) &destino, 
		sizeof(destino));

    if(rc<0) {
      printf("cannot send data \n");
      close(sd);
      exit(1);
    }
}
