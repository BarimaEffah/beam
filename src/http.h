#ifndef BEAM_HTTP_H
#define BEAM_HTTP_H

#define HTTP_DEFAULT_PORT 80
#define MAX_HTTP_BUFFER_SIZE 65536
#define MAX_HTTP_HEADER_SIZE 16384

typedef struct
{
    char *scheme;
    char *host;
    char *port;
    char *path;
    char *query;
    char *fragment;
} url_t;
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
    url_t *url_t;
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

typedef struct http_request request_opts;

http_request *new_request(http_request *opts);

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
void http_post(http_request *request);
void http_get(http_request *request);
void http_put(http_request *request);
void http_delete(http_request *request);
void http_head(http_request *request);
void http_options(http_request *request);

void make_http_request(http_request *request, http_response *response);

void parse_json(char *json, char **dest);
void parse_url(char *url, url_t *dest);
void free_url(url_t *url);

#endif
