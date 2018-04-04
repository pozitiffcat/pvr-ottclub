#include "XBMCHttpRequestBuilder.h"

XBMCHttpRequestBuilder::XBMCHttpRequestBuilder(ADDON::CHelper_libXBMC_addon *XBMC)
    : XBMC(XBMC)
{
}

std::string XBMCHttpRequestBuilder::doGetRequest(const std::string &url) const
{
    void *file = XBMC->OpenFile(url.c_str(), 0);

    if (!file)
        return std::string();

    char buffer[1024];
    memset(buffer, 0, 1024);

    std::string result;

    while (XBMC->ReadFile(file, buffer, sizeof(buffer) - 1) > 0)
        result += buffer;

    XBMC->CloseFile(file);

    return result;
}
