#include "lab.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static request_t req(const char *ct){
 request_t r;memset(&r,0,sizeof(r));strcpy(r.method,"POST");
 if(ct){strncpy(r.content_type,ct,sizeof(r.content_type)-1);r.has_content_type=1;}return r;
}
int main(void){
 char why[256];
 assert(validate_multipart("multipart/form-data; boundary=abc_123",why,sizeof(why)));
 assert(!validate_multipart("text/plain",why,sizeof(why)));
 assert(!validate_multipart("multipart/form-data",why,sizeof(why)));
 assert(!validate_multipart("multipart/form-data; boundary=bad!x",why,sizeof(why)));
 request_t good=req("multipart/form-data; boundary=abc_123");result_t o;
 handle_request(&good,VULNERABLE_MODE,&o);assert(o.status==200);
 handle_request(&good,PATCHED_MODE,&o);assert(o.status==200);
 request_t bad=req("application/x-invalid; [LAB_CONTROLLED_MARKER]");
 handle_request(&bad,VULNERABLE_MODE,&o);
 assert(o.status==500 && o.reflected_untrusted_input);
 assert(strstr(o.response_body,"LAB_CONTROLLED_MARKER")!=NULL);
 handle_request(&bad,PATCHED_MODE,&o);
 assert(o.status==400 && !o.reflected_untrusted_input);
 assert(strstr(o.response_body,"LAB_CONTROLLED_MARKER")==NULL);
 request_t missing=req(NULL);handle_request(&missing,PATCHED_MODE,&o);assert(o.status==400);
 puts("8 testes passaram.");return 0;
}
