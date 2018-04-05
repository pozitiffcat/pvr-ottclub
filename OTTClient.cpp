#include "OTTClient.h"

#include <sstream>
#include <iostream>

#include "jparsers/jparsers.h"
#include "jparsers/jobject.h"
#include "jparsers/jarray.h"

#include "HttpRequestBuilder.h"

OTTClient::OTTClient(HttpRequestBuilder *httpRequestBuilder, const std::string &key)
    : m_httpRequestBuilder(httpRequestBuilder),
      m_key(key)
{
}

void OTTClient::setKey(const std::string &key)
{
    m_key = key;
}

void OTTClient::fetchChannels()
{
    m_groups.clear();

    if (m_key.empty())
        return;

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
            const jobject *categoryObject = channelObject->value("category")->as_object();

            Channel channel;
            channel.id = channelObject->value("ch_id")->as_string();
            channel.name = channelObject->value("channel_name")->as_string();
            channel.url = "http://spacetv.in/stream/" + m_key + "/" + channel.id + ".m3u8";
            channel.icon = "http://ott.watch/images/" + channelObject->value("img")->as_string();

            Group *group = groupByName(categoryObject->value("name")->as_string());

            if (group)
            {
                group->channels.push_back(channel);
            }
            else
            {
                Group newGroup;
                newGroup.clazz = categoryObject->value("class")->as_string();
                newGroup.name = categoryObject->value("name")->as_string();
                newGroup.channels.push_back(channel);
                m_groups.push_back(newGroup);
            }
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

    channel->programs.clear();

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
    int count = 0;

    for (int i = 0; i < m_groups.size(); ++i)
        count += m_groups[i].channels.size();

    return count;
}

OTTClient::Channel *OTTClient::channelByIndex(int index)
{
    int pos = 0;

    for (int i = 0; i < m_groups.size(); ++i)
    {
        if (pos + m_groups[i].channels.size() > index)
            return &m_groups[i].channels[index - pos];

        pos += m_groups[i].channels.size();
    }

    return NULL;
}

OTTClient::Channel *OTTClient::channelById(const std::string &id)
{
    for (int i = 0; i < m_groups.size(); ++i)
    {
        for (int j = 0; j < m_groups[i].channels.size(); ++j)
        {
            if (m_groups[i].channels[j].id == id)
                return &m_groups[i].channels[j];
        }
    }

    return NULL;
}

int OTTClient::groupsCount() const
{
    return m_groups.size();
}

OTTClient::Group *OTTClient::groupByIndex(int index)
{
    if (index < 0 || index >= m_groups.size())
        return NULL;

    return &m_groups[index];
}

OTTClient::Group *OTTClient::groupByName(const std::string &name)
{
    for (int i = 0; i < m_groups.size(); ++i)
    {
        if (m_groups[i].name == name)
            return &m_groups[i];
    }

    return NULL;
}
