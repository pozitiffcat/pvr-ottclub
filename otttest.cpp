#include "OTTClient.h"

int main()
{
    OTTClient c;
    c.fetchChannels();

    for (int i = 0; i < c.channelsCount(); ++i)
    {
        c.fetchPrograms(c.channel(i).id);
    }

    return 0;
}
