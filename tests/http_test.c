#include "test.h"
#include "../src/http.h"

TEST(encode_request)
{
    http_request opts = {
        .url = "google.com",
        .content_type = "application/json",
        // .headers = {"Authorization: Bearer 1234567890", "X-API-Key: 1234567890"},
        .headers_count = 2,
    };

    opts.headers = malloc(sizeof(char *) * opts.headers_count);
    opts.headers[0] = strdup("Authorization: Bearer 1234567890");
    opts.headers[1] = strdup("Authorization: Bearer 1234567890");

    http_request *req = new_request(&opts);
    char *request = encode_request(req);
    char *expected = "GET google.com HTTP/1.1\r\nHost:google.com\r\nUser-Agent:BEAM\r\nAccept:*/*\r\nContent-Type:application/json\r\nContent-Length:0\r\nAuthorization:Bearer 1234567890\r\nX-API-Key:1234567890\r\n\r\n";
    ASSERT_EQ_STR(expected, request);
    free_request(req);
}
