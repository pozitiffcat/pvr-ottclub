#ifndef OTT_CLIENT_H
#define OTT_CLIENT_H

#include <map>
#include <string>
#include <vector>

class HttpRequestBuilder;

class OTTClient
{
public:
    struct Program
    {
        std::string name;
        time_t time;
        time_t timeTo;
        std::string duration;
        std::string description;
    };

    struct Channel
    {
        std::string id;
        std::string name;
        std::string url;
        std::string icon;
        std::vector<Program> programs;
    };

    struct Group
    {
        std::string clazz;
        std::string name;
        std::vector<Channel> channels;
    };

    struct GroupChannelIndex
    {
        int groupIndex;
        int channelIndex;
    };

public:
    OTTClient(HttpRequestBuilder *httpRequestBuilder, const std::string &key);

    void setKey(const std::string &key);

    void fetchChannels();
    void fetchPrograms(const std::string &channelId);

    int channelsCount() const;
    Channel *channelByIndex(int index);
    Channel *channelById(const std::string &id);

    int groupsCount() const;
    Group *groupByIndex(int index);
    Group *groupByName(const std::string &id);

private:
    HttpRequestBuilder *m_httpRequestBuilder;
    std::string m_key;
    std::vector<Group> m_groups;

    std::map<std::string, GroupChannelIndex> m_channelByIdIndexes;
    std::vector<GroupChannelIndex> m_channelIndexes;
};

#endif // OTT_CLIENT_H
