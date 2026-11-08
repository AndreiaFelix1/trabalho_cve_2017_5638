#ifndef LAB_H
#define LAB_H
#include <stddef.h>
#define CT_MAX 1024
#define BODY_MAX 4096

typedef enum { VULNERABLE_MODE=0, PATCHED_MODE=1 } mode_t_lab;
typedef struct {
    char method[16], path[256], content_type[CT_MAX];
    int has_content_type;
    char body[BODY_MAX];
} request_t;
typedef struct {
    int status;
    int reflected_untrusted_input;
    char response_body[2048];
    char internal_log[1024];
} result_t;

int parse_request(const char *raw, request_t *req);
int validate_multipart(const char *content_type, char *reason, size_t reason_size);
int handle_request(const request_t *req, mode_t_lab mode, result_t *result);
void make_response(const result_t *result, char *out, size_t out_size);
#endif
