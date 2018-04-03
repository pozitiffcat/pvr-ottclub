#ifndef OTT_CLIENT_H
#define OTT_CLIENT_H

#include <string>
#include <vector>

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
        std::vector<Program> programs;
    };

public:
    OTTClient();

    void fetch();

    Channel channel(int index) const;
    Channel channelById(const std::string &id) const;

    int channelsCount() const;

private:
    std::vector<Channel> m_channels;
};

#endif // OTT_CLIENT_H
