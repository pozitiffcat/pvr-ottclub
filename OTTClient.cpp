#include "OTTClient.h"

#include <sstream>
#include <iostream>
#include <curl/curl.h>
#include "jparsers/jparsers.h"
#include "jparsers/jobject.h"
#include "jparsers/jarray.h"

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

void OTTClient::fetchChannels()
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

    jobject *rootJson = 0;

    try
    {
        rootJson = jparsers::read_object(bufferStream);
    }
    catch (const std::runtime_error &)
    {
        return;
    }

    std::vector<std::pair<std::string, jvalue *> > pairs = rootJson->to_pairs();
    for (std::vector<std::pair<std::string, jvalue *> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
    {
        try
        {
            const jobject *channelObject = it->second->as_object();
            Channel channel;
            channel.id = channelObject->value("ch_id")->as_string();
            channel.name = channelObject->value("channel_name")->as_string();
            channel.url = "http://spacetv.in/stream/BES5W7VUMB/" + channel.id + ".m3u8";
            channel.icon = "http://ott.watch/images/" + channelObject->value("img")->as_string();
            m_channels.push_back(channel);
        }
        catch (const std::runtime_error &)
        {
        }
    }

    delete rootJson;
}

OTTClient::Channel OTTClient::fetchPrograms(const std::string &channelId)
{
    Channel *channel;

    for (int i = 0; i < m_channels.size(); ++i)
    {
        if (m_channels[i].id == channelId)
        {
            channel = &m_channels[i];
            break;
        }
    }

    if (!channel->isValid())
        return Channel();

    std::string buffer;

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, ("http://ott.watch/api/channel/" + channel->id).c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::stringstream bufferStream(buffer);

    jobject *rootJson = 0;
    const jarray *arrayJson = 0;

    try
    {
        rootJson = jparsers::read_object(bufferStream);
        arrayJson = rootJson->value("epg_data")->as_array();
    }
    catch (const std::runtime_error &)
    {
        return Channel();
    }

    for (int i = 0; i < arrayJson->count(); ++i)
    {
        try
        {
            const jobject *programJson = arrayJson->value(i)->as_object();
            Program program;
            program.name = programJson->value("name")->as_string();
            program.duration = programJson->value("duration")->as_string();
            program.description = programJson->value("descr")->as_string();
            program.time = programJson->value("time")->as_number();
            program.timeTo = programJson->value("time_to")->as_number();
            channel->programs.push_back(program);
        }
        catch (const std::runtime_error &)
        {
        }
    }

    delete rootJson;
    return *channel;
}

const OTTClient::Channel &OTTClient::channel(int index) const
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
