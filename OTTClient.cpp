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

            int groupIndex = -1;
            Group *group = groupByName(categoryObject->value("name")->as_string());

            for (int i = 0; i < m_groups.size(); ++i)
            {
                if (m_groups[i].name == categoryObject->value("name")->as_string())
                {
                    groupIndex = i;
                    group = &m_groups[i];
                    break;
                }
            }

            if (group)
            {
                group->channels.push_back(channel);

                GroupChannelIndex index;
                index.groupIndex = groupIndex;
                index.channelIndex = group->channels.size() - 1;
                m_channelIndexes.push_back(index);
                m_channelByIdIndexes[channel.id] = index;
            }
            else
            {
                Group newGroup;
                newGroup.clazz = categoryObject->value("class")->as_string();
                newGroup.name = categoryObject->value("name")->as_string();
                newGroup.channels.push_back(channel);
                m_groups.push_back(newGroup);

                GroupChannelIndex index;
                index.groupIndex = m_groups.size() - 1;
                index.channelIndex = 0;
                m_channelIndexes.push_back(index);
                m_channelByIdIndexes[channel.name] = index;
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
    return m_channelIndexes.size();
}

OTTClient::Channel *OTTClient::channelByIndex(int index)
{
    const int groupIndex = m_channelIndexes[index].groupIndex;
    const int channelIndex = m_channelIndexes[index].channelIndex;

    if (groupIndex < 0 || groupIndex >= m_groups.size())
        return NULL;

    if (channelIndex < 0 || channelIndex >= m_groups[groupIndex].channels.size())
        return NULL;

    return &m_groups[groupIndex].channels[channelIndex];
}

OTTClient::Channel *OTTClient::channelById(const std::string &id)
{
    std::map<std::string, GroupChannelIndex>::iterator it = m_channelByIdIndexes.find(id);
    if (it == m_channelByIdIndexes.end())
        return NULL;

    const int groupIndex = it->second.groupIndex;
    const int channelIndex = it->second.channelIndex;

    if (groupIndex < 0 || groupIndex >= m_groups.size())
        return NULL;

    if (channelIndex < 0 || channelIndex >= m_groups[groupIndex].channels.size())
        return NULL;

    return &m_groups[groupIndex].channels[channelIndex];
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
