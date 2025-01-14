#ifndef BEAM_HTTP_H
#define BEAM_HTTP_H

#define HTTP_DEFAULT_PORT 80
#define MAX_HTTP_BUFFER_SIZE 65536

typedef enum http_method
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
} http_method;

char *http_method_to_string(http_method method);

typedef struct
{
    char *http_version;
    char *host;
    http_method method;
    char *url;
    char *content_type;
    int content_length;
    char *body;
    char *user_agent;
    char *accept;
    char *encoding;
    char **headers;
    int headers_count;
} http_request;

void free_request(http_request *request);

typedef struct
{
    int status;
    char *status_text;
    char *content_type;
    char *body;
} http_response;

void parse_response(char *response, http_response *dest);

char *encode_request(http_request *request);
char *encode_headers(char **headers, int count);
static void http_get(http_request *request);
static void http_post(http_request *request);
static void http_put(http_request *request);
static void http_delete(http_request *request);
static void http_head(http_request *request);
static void http_options(http_request *request);

void make_http_request(http_request *request, http_response *response);

void parse_json(char *json, char **dest);

#endif