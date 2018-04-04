#ifndef CURL_HTTP_REQUEST_BUILDER_H
#define CURL_HTTP_REQUEST_BUILDER_H

#include "HttpRequestBuilder.h"

class CurlHttpRequestBuilder : public HttpRequestBuilder
{
public:
    std::string doGetRequest(const std::string &url) const;
};

#endif // CURL_HTTP_REQUEST_BUILDER_H
