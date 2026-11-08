#define _POSIX_C_SOURCE 200809L
#include "lab.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define REQ_CAP 8192
int main(int argc,char **argv) {
    int patched=0,port=8080;
    if(argc>1 && strcmp(argv[1],"--patched")==0) patched=1;
    if(argc>2) port=atoi(argv[2]);
    if(port<1024 || port>65535){fprintf(stderr,"Porta invalida.\n");return 1;}
    int s=socket(AF_INET,SOCK_STREAM,0); if(s<0){perror("socket");return 1;}
    int yes=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port);
    inet_pton(AF_INET,"127.0.0.1",&a.sin_addr); /* somente loopback */
    if(bind(s,(struct sockaddr*)&a,sizeof(a))<0 || listen(s,16)<0){
        perror("bind/listen");close(s);return 1;
    }
    printf("Servidor %s em 127.0.0.1:%d\n",patched?"MITIGADO":"SIMULACAO",port);
    for(;;){
        int c=accept(s,NULL,NULL); if(c<0){if(errno==EINTR)continue;perror("accept");break;}
        char raw[REQ_CAP],resp[4096]; ssize_t n=recv(c,raw,sizeof(raw)-1,0);
        if(n>0){
            raw[n]='\0'; request_t req; result_t res;
            if(parse_request(raw,&req)==0) handle_request(&req,patched?PATCHED_MODE:VULNERABLE_MODE,&res);
            else {memset(&res,0,sizeof(res));res.status=400;strcpy(res.response_body,"HTTP invalido.\n");}
            printf("status=%d refletiu_entrada=%s | %s\n",res.status,
                   res.reflected_untrusted_input?"sim":"nao",res.internal_log);
            make_response(&res,resp,sizeof(resp)); send(c,resp,strlen(resp),0);
        }
        close(c);
    }
    close(s); return 0;
}
