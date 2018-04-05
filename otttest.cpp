#include "OTTClient.h"

#include "CurlHttpRequestBuilder.h"

int main()
{
    CurlHttpRequestBuilder httpBuilder;
    OTTClient c(&httpBuilder, "TEST");
    c.fetchChannels();

    for (int i = 0; i < c.channelsCount(); ++i)
    {
        c.fetchPrograms(c.channelByIndex(i)->id);
    }

    return 0;
}
