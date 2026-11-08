#define _POSIX_C_SOURCE 200809L
#include "lab.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define ITERATIONS 200000
static double run(mode_t_lab mode,const request_t *r){
    struct timespec a,b; result_t out;
    clock_gettime(CLOCK_MONOTONIC,&a);
    for(int i=0;i<ITERATIONS;i++) handle_request(r,mode,&out);
    clock_gettime(CLOCK_MONOTONIC,&b);
    double sec=(b.tv_sec-a.tv_sec)+(b.tv_nsec-a.tv_nsec)/1e9;
    return sec*1e6/ITERATIONS;
}
int main(void){
    request_t valid,bad;memset(&valid,0,sizeof(valid));memset(&bad,0,sizeof(bad));
    strcpy(valid.method,"POST");strcpy(valid.content_type,"multipart/form-data; boundary=Bench_123");valid.has_content_type=1;
    strcpy(bad.method,"POST");strcpy(bad.content_type,"application/x-invalid; [LAB_CONTROLLED_MARKER]");bad.has_content_type=1;
    printf("Iteracoes por cenario: %d\n",ITERATIONS);
    printf("Valida - vulneravel: %.3f us/requisicao\n",run(VULNERABLE_MODE,&valid));
    printf("Valida - mitigado:   %.3f us/requisicao\n",run(PATCHED_MODE,&valid));
    printf("Invalida - vulneravel: %.3f us/requisicao\n",run(VULNERABLE_MODE,&bad));
    printf("Invalida - mitigado:   %.3f us/requisicao\n",run(PATCHED_MODE,&bad));
    puts("Medicao local da funcao de tratamento; nao representa latencia de rede ou Apache Struts.");
    return 0;
}
