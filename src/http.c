#include "http.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void free_request(http_request *request)
{
    if (request == NULL)
    {
        return;
    }
    free(request->http_version);
    free(request->host);
    free(request->url);
    free(request->content_type);
    free(request->body);
    free(request->user_agent);
    free(request->accept);
    free(request->encoding);
    for (int i = 0; i < request->headers_count; i++)
    {
        free(request->headers[i]);
    }
    free(request->headers);
    free(request);
}

http_request *new_request(http_request *opts)
{
    if (opts->url == NULL)
    {
        errno = EINVAL;
        perror("url is required");
        return NULL;
    }

    http_request *request = malloc(sizeof(http_request));

    if (request == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for request");
        return NULL;
    }

    request->http_version = strdup(opts->http_version ? opts->http_version : "HTTP/1.1");
    request->host = strdup(opts->host ? opts->host : "localhost");
    request->method = opts->method ? opts->method : GET;
    request->url = strdup(opts->url);
    request->content_type = strdup(opts->content_type ? opts->content_type : "application/json");
    request->content_length = opts->content_length ? opts->content_length : 0;
    request->body = strdup(opts->body ? opts->body : "");
    request->user_agent = strdup(opts->user_agent ? opts->user_agent : "BEAM");
    request->accept = strdup(opts->accept ? opts->accept : "*/*");
    request->encoding = strdup(opts->encoding ? opts->encoding : "");
    request->headers = opts->headers;
    request->headers_count = opts->headers_count ? opts->headers_count : 0;
    return request;
}

char *http_method_to_string(http_method method)
{
    switch (method)
    {
    case GET:
        return "GET";
    case POST:
        return "POST";
    case PUT:
        return "PUT";
    case DELETE:
        return "DELETE";
    case HEAD:
        return "HEAD";
    case OPTIONS:
        return "OPTIONS";
    default:
        return "UNKNOWN";
    }
}

char *encode_headers(char **headers, int count)
{
    char *buffer = malloc(MAX_HTTP_HEADER_SIZE);
    if (buffer == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for buffer");
        return NULL;
    }
    size_t offset = 0;
    for (int i = 0; i < count; i++)
    {
        int print_len = snprintf(buffer + offset, MAX_HTTP_HEADER_SIZE, "%s\r\n", headers[i]);
        if (print_len < 0)
        {
            perror("could not encode headers");
            free(buffer);
            return NULL;
        }
        if (offset + print_len >= MAX_HTTP_HEADER_SIZE)
        {
            printf("Buffer too small. Maximum is %d bytes, got %lu", MAX_HTTP_HEADER_SIZE, offset + print_len);
            free(buffer);
            return NULL;
        }
        offset += print_len;
    }
    return buffer;
}

char *encode_request(http_request *request)
{
    char *buffer = malloc(MAX_HTTP_BUFFER_SIZE);
    if (buffer == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for buffer");
        return NULL;
    }

    char *headers = NULL;

    if (request->headers_count > 0)
    {
        headers = encode_headers(request->headers, request->headers_count);
        if (headers == NULL)
        {
            free(buffer);
            return NULL;
        }
    }

    printf("Headers(%d): %s\n", request->headers_count, headers);

    int print_len = snprintf(buffer, MAX_HTTP_BUFFER_SIZE, "%s %s HTTP/1.1\r\nHost:%s\r\nUser-Agent:%s\r\nAccept:%s\r\nContent-Type:%s\r\nContent-Length:%d\r\n%s\r\n%s", http_method_to_string(request->method), request->url, request->host, request->user_agent, request->accept, request->content_type, request->content_length, headers ? headers : "", request->body ? request->body : "");

    if (print_len >= MAX_HTTP_BUFFER_SIZE)
    {
        printf("Buffer too small. Maximum is %d bytes, got %d", MAX_HTTP_BUFFER_SIZE, print_len);
        free(buffer);
        return NULL;
    }
    if (print_len < 0)
    {
        perror("could not encode request");
        free(buffer);
        return NULL;
    }
    return buffer;
}
