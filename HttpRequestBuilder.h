#ifndef HTTP_REQUEST_BUILDER_H
#define HTTP_REQUEST_BUILDER_H

#include <string>

class HttpRequestBuilder
{
public:
    virtual ~HttpRequestBuilder() {}
    virtual std::string doGetRequest(const std::string &url) const = 0;
};

#endif // HTTP_REQUEST_BUILDER_H
