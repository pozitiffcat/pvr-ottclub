#include "CurlHttpRequestBuilder.h"

#include <curl/curl.h>

static int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
    if(writerData == NULL)
        return 0;

    writerData->append(data, size * nmemb);
    return size * nmemb;
}

std::string CurlHttpRequestBuilder::doGetRequest(const std::string &url) const
{
    std::string buffer;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return buffer;
}
