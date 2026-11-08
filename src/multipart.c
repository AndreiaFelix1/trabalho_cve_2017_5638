#include "lab.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

/*
 * Validador reduzido de Content-Type multipart. Não é o Jakarta Multipart
 * nem pretende implementar todas as regras da RFC 7578.
 */
int validate_multipart(const char *ct, char *reason, size_t cap) {
    if (!ct || !*ct) { snprintf(reason,cap,"Content-Type ausente"); return 0; }
    while (*ct==' ' || *ct=='\t') ct++;
    if (strncasecmp(ct,"multipart/form-data",19)!=0 ||
        (ct[19] && ct[19]!=';' && ct[19]!=' ' && ct[19]!='\t')) {
        snprintf(reason,cap,"tipo de midia nao e multipart/form-data"); return 0;
    }
    const char *p=ct+19;
    while (*p) {
        while (*p==' ' || *p=='\t' || *p==';') p++;
        if (!*p) break;
        const char *key=p;
        while (*p && *p!='=' && *p!=';') p++;
        if (*p!='=') { while(*p && *p!=';') p++; continue; }
        size_t keylen=(size_t)(p-key); p++;
        if (keylen==8 && strncasecmp(key,"boundary",8)==0) {
            if (*p=='"') p++;
            int count=0;
            while (*p && *p!='"' && *p!=';' && *p!='\r' && *p!='\n') {
                unsigned char c=(unsigned char)*p;
                if (!(isalnum(c)||c=='-'||c=='_')) {
                    snprintf(reason,cap,"boundary contem caractere nao permitido"); return 0;
                }
                count++; p++;
            }
            if (count>0 && count<=70) { snprintf(reason,cap,"multipart valido"); return 1; }
            snprintf(reason,cap,"boundary vazio ou longo demais"); return 0;
        }
        while (*p && *p!=';') p++;
    }
    snprintf(reason,cap,"parametro boundary ausente"); return 0;
}
