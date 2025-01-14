#include "http.h"
#include <stdlib.h>
#include <stdio.h>

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

char *encode_headers(char **headers, int count) {}

char *encode_request(http_request *request)
{
    char *buffer = malloc(MAX_HTTP_BUFFER_SIZE);
    if (buffer == NULL)
    {
        perror("could not allocate memory for buffer");
        return NULL;
    }

    char *headers = encode_headers(request->headers, request->headers_count);

    int print_len = snprintf(buffer, MAX_HTTP_BUFFER_SIZE, "%s %s HTTP/1.1\r\nHost:%s\r\nUser-Agent:%s\r\nAccept:%s\r\nContent-Type:%s\r\nContent-Length:%d\r\n\r\n%s", http_method_to_string(request->method), request->url, request->host, request->user_agent, request->accept, request->content_type, request->content_length, request->body);

    if (print_len >= MAX_HTTP_BUFFER_SIZE)
    {
        printf("Buffer too small. Maximum is %d bytes, got %d", MAX_HTTP_BUFFER_SIZE, print_len);
        return NULL;
    }
    if (print_len < 0 || print_len >= MAX_HTTP_BUFFER_SIZE)
    {
        perror("could not encode request");
        return NULL;
    }
    return buffer;
}