#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc,char **argv){
    int port=8080,bad=0;
    if(argc>1 && strcmp(argv[1],"--invalid")==0) bad=1;
    if(argc>2) port=atoi(argv[2]);
    int fd=socket(AF_INET,SOCK_STREAM,0); if(fd<0){perror("socket");return 1;}
    struct sockaddr_in a;memset(&a,0,sizeof(a));a.sin_family=AF_INET;
    a.sin_port=htons((unsigned short)port);inet_pton(AF_INET,"127.0.0.1",&a.sin_addr);
    if(connect(fd,(struct sockaddr*)&a,sizeof(a))<0){perror("connect");close(fd);return 1;}
    const char *good="POST /upload HTTP/1.1\r\nHost: localhost\r\n"
      "Content-Type: multipart/form-data; boundary=LabBoundary_123\r\n"
      "Content-Length: 0\r\nConnection: close\r\n\r\n";
    const char *badreq="POST /upload HTTP/1.1\r\nHost: localhost\r\n"
      "Content-Type: application/x-invalid; [LAB_CONTROLLED_MARKER]\r\n"
      "Content-Length: 0\r\nConnection: close\r\n\r\n";
    const char *q=bad?badreq:good; send(fd,q,strlen(q),0);
    char b[2048];ssize_t n;while((n=recv(fd,b,sizeof(b)-1,0))>0){b[n]='\0';fputs(b,stdout);}
    close(fd);return 0;
}
