#include "OTTClient.h"

int main()
{
    OTTClient client;
    client.fetchChannels();
    client.fetchPrograms();
    return 0;
}
