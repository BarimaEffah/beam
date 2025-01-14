#ifndef BEAM_HTTP_H
#define BEAM_HTTP_H

typedef enum http_method
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
} http_method;
typedef struct
{
    char *http_version;
    http_method method;
    char *url;
    char *content_type;
    int content_length;
    char *body;
    char *user_agent;
    char *accept;
    char *encoding;
    char **headers;
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

static void http_get(http_request *request);
static void http_post(http_request *request);
static void http_put(http_request *request);
static void http_delete(http_request *request);
static void http_head(http_request *request);
static void http_options(http_request *request);

void make_http_request(http_request *request, http_response *response);

void parse_json(char *json, char **dest);

#endif