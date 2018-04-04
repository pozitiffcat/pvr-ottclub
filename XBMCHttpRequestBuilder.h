#ifndef XBMC_HTTP_REQUEST_BUILDER_H
#define XBMC_HTTP_REQUEST_BUILDER_H

#include "HttpRequestBuilder.h"

#include "libXBMC_addon.h"
#include "HttpRequestBuilder.h"

class XBMCHttpRequestBuilder : public HttpRequestBuilder
{
public:
    explicit XBMCHttpRequestBuilder(ADDON::CHelper_libXBMC_addon *XBMC);

    std::string doGetRequest(const std::string &url) const;

private:
    ADDON::CHelper_libXBMC_addon *XBMC;
};

#endif // XBMC_HTTP_REQUEST_BUILDER_H
