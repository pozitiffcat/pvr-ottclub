#include "OTTClient.h"

#include "CurlHttpRequestBuilder.h"

int main()
{
    CurlHttpRequestBuilder httpBuilder;
    OTTClient c(&httpBuilder);
    c.fetchChannels();

    for (int i = 0; i < c.channelsCount(); ++i)
    {
        c.fetchPrograms(c.channel(i).id);
    }

    return 0;
}
