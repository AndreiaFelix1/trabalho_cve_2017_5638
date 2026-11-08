#include "lab.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

/*
 * Simula o caminho de erro do processamento multipart.
 * O marcador [LAB_CONTROLLED_MARKER] é texto inerte criado para este projeto.
 * Não é OGNL, não é interpretado e não executa comandos.
 */
int handle_request(const request_t *req, mode_t_lab mode, result_t *out) {
    if (!req || !out) return -1;
    memset(out,0,sizeof(*out));
    if (strcasecmp(req->method,"POST")!=0) {
        out->status=405;
        snprintf(out->response_body,sizeof(out->response_body),"Metodo nao permitido.\n");
        snprintf(out->internal_log,sizeof(out->internal_log),"metodo rejeitado");
        return 0;
    }
    char reason[256] = "Content-Type ausente";
    int valid=req->has_content_type &&
              validate_multipart(req->content_type,reason,sizeof(reason));
    int marker=strstr(req->content_type,"[LAB_CONTROLLED_MARKER]")!=NULL;

    if (!valid || marker) {
        /*
         * Modo vulnerável: representa a falha conceitual de construir
         * diagnóstico/erro usando diretamente dado não confiável.
         * A reflexão é apenas evidência local de propagação de entrada.
         */
        if (mode==VULNERABLE_MODE) {
            out->status=500;
            out->reflected_untrusted_input=1;
            snprintf(out->response_body,sizeof(out->response_body),
                     "Erro no processamento multipart: %.700s\n",
                     req->has_content_type ? req->content_type : "(ausente)");
            snprintf(out->internal_log,sizeof(out->internal_log),
                     "SIMULACAO: falha multipart; motivo=%s",reason);
        } else {
            out->status=400;
            out->reflected_untrusted_input=0;
            snprintf(out->response_body,sizeof(out->response_body),
                     "Requisicao multipart invalida.\n");
            snprintf(out->internal_log,sizeof(out->internal_log),
                     "MITIGADO: entrada rejeitada; motivo interno=%s",reason);
        }
        return 0;
    }
    out->status=200;
    snprintf(out->response_body,sizeof(out->response_body),
             "Requisicao multipart aceita.\n");
    snprintf(out->internal_log,sizeof(out->internal_log),"multipart aceito");
    return 0;
}
