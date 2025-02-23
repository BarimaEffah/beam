#include "test.h"
#include "../src/http.h"

TEST(parse_url)
{
    char *url = "https://google.com/path/to/file?query=param&another=value";
    url_t *parsed = malloc(sizeof(url_t));
    parse_url(url, parsed);
    ASSERT_EQ_STR("google.com", parsed->host);
    ASSERT_EQ_STR("/path/to/file", parsed->path);
    ASSERT_EQ_STR("query=param&another=value", parsed->query);
    free_url(parsed);
}

TEST(encode_request)
{
    http_request opts = {
        .url = "google.com",
        .content_type = "application/json",
        .headers_count = 2,
        .user_agent = "BEAM_TEST"};

    opts.headers = malloc(sizeof(char *) * opts.headers_count);
    opts.headers[0] = strdup("Authorization:Bearer 1234567890");
    opts.headers[1] = strdup("X-API-Key:1234567890");

    http_request *req = new_request(&opts);
    char *request = encode_request(req);
    char *expected = "GET / HTTP/1.1\r\nHost:google.com\r\nUser-Agent:BEAM_TEST\r\nAccept:*/*\r\nContent-Type:application/json\r\nContent-Length:0\r\nAuthorization:Bearer 1234567890\r\nX-API-Key:1234567890\r\n\r\n";
    ASSERT_EQ_STR(expected, request);
    free_request(req);
}
