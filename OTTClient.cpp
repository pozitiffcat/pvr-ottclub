#include "OTTClient.h"

#include <sstream>
#include <iostream>
#include <curl/curl.h>
#include "json/json.h"

static int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
    if(writerData == NULL)
        return 0;

    writerData->append(data, size * nmemb);
    return size * nmemb;
}

OTTClient::OTTClient()
{
}

void OTTClient::fetch()
{
    std::string buffer;

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://ott.watch/api/channel_now");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::stringstream bufferStream(buffer);

    Json::Value rootJson;
    bufferStream >> rootJson;

    for (Json::Value::iterator it = rootJson.begin(); it != rootJson.end(); ++it)
    {
        const Json::Value &channelJson = *it;

        Channel channel;
        channel.id = channelJson["ch_id"].asString();
        channel.name = channelJson["channel_name"].asString();
        channel.url = "http://spacetv.in/stream/BES5W7VUMB/" + channel.id + ".m3u8";

        Program program;
        program.name = channelJson["name"].asString();
        program.duration = channelJson["duration"].asString();
        program.description = channelJson["descr"].asString();
        program.time = channelJson["time"].asInt64();;
        program.timeTo = channelJson["time_to"].asInt64();

        channel.programs.push_back(program);
        m_channels.push_back(channel);
    }
}

OTTClient::Channel OTTClient::channel(int index) const
{
    return m_channels[index];
}

OTTClient::Channel OTTClient::channelById(const std::string &id) const
{
    for (int i = 0; i < m_channels.size(); ++i)
    {
        if (m_channels[i].id == id)
            return m_channels[i];
    }

    return Channel();
}

int OTTClient::channelsCount() const
{
    return m_channels.size();
}