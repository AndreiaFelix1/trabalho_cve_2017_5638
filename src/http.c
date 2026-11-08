#define _POSIX_C_SOURCE 200809L
#include "lab.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

/* Parser HTTP pequeno para laboratório: uma requisição por conexão. */
int parse_request(const char *raw, request_t *req) {
    if (!raw || !req) return -1;
    memset(req, 0, sizeof(*req));
    const char *line = strstr(raw, "\r\n");
    if (!line || (size_t)(line-raw) >= 512) return -1;
    char first[512];
    size_t first_len=(size_t)(line-raw);
    memcpy(first,raw,first_len); first[first_len]='\0';
    if (sscanf(first,"%15s %255s",req->method,req->path)!=2) return -1;

    const char *p=line+2, *headers_end=strstr(p,"\r\n\r\n");
    if (!headers_end) return -1;
    while (p < headers_end) {
        const char *eol=strstr(p,"\r\n");
        if (!eol || eol>headers_end) break;
        const char *colon=memchr(p,':',(size_t)(eol-p));
        if (colon && (size_t)(colon-p)==12 && strncasecmp(p,"Content-Type",12)==0) {
            const char *v=colon+1;
            while(v<eol && (*v==' ' || *v=='\t')) v++;
            size_t n=(size_t)(eol-v);
            if(n>=sizeof(req->content_type)) n=sizeof(req->content_type)-1;
            memcpy(req->content_type,v,n); req->content_type[n]='\0';
            req->has_content_type=1;
        }
        p=eol+2;
    }
    const char *body=headers_end+4;
    size_t n=strlen(body);
    if(n>=sizeof(req->body)) n=sizeof(req->body)-1;
    memcpy(req->body,body,n); req->body[n]='\0';
    return 0;
}
