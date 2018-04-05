#include "OTTClient.h"

#include <sstream>
#include <iostream>

#include "jparsers/jparsers.h"
#include "jparsers/jobject.h"
#include "jparsers/jarray.h"

#include "HttpRequestBuilder.h"

OTTClient::OTTClient(HttpRequestBuilder *httpRequestBuilder)
    : m_httpRequestBuilder(httpRequestBuilder)
{
}

void OTTClient::fetchChannels()
{
    std::string buffer = m_httpRequestBuilder->doGetRequest("http://ott.watch/api/channel_now");
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

void OTTClient::fetchPrograms(const std::string &channelId)
{
    Channel *channel = channelById(channelId);
    if (!channel)
        return;

    std::string buffer = m_httpRequestBuilder->doGetRequest("http://ott.watch/api/channel/" + channel->id);
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
        return;
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
}

int OTTClient::channelsCount() const
{
    return m_channels.size();
}

OTTClient::Channel *OTTClient::channelByIndex(int index)
{
    if (index < 0 || index >= m_channels.size())
        return NULL;

    return &m_channels[index];
}

OTTClient::Channel *OTTClient::channelById(const std::string &id)
{
    for (int i = 0; i < m_channels.size(); ++i)
    {
        if (m_channels[i].id == id)
            return &m_channels[i];
    }

    return NULL;
}
