#include "lab.h"
#include <stdio.h>
#include <string.h>
void make_response(const result_t *r,char *out,size_t cap) {
    const char *reason=r->status==200?"OK":r->status==400?"Bad Request":
                       r->status==405?"Method Not Allowed":"Internal Server Error";
    snprintf(out,cap,"HTTP/1.1 %d %s\r\nContent-Type: text/plain; charset=utf-8\r\n"
        "X-Content-Type-Options: nosniff\r\nContent-Length: %zu\r\n"
        "Connection: close\r\n\r\n%s",r->status,reason,
        strlen(r->response_body),r->response_body);
}
