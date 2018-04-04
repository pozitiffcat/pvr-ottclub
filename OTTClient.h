#ifndef OTT_CLIENT_H
#define OTT_CLIENT_H

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

        bool isValid() const
        {
            return !id.empty();
        }
    };

public:
    explicit OTTClient(HttpRequestBuilder *httpRequestBuilder);

    void fetchChannels();
    Channel fetchPrograms(const std::string &channelId);

    const Channel &channel(int index) const;
    Channel &channelById(const std::string &id, const Channel &def);
    Channel channelById(const std::string &id) const;

    int channelsCount() const;

private:
    HttpRequestBuilder *m_httpRequestBuilder;
    std::vector<Channel> m_channels;
};

#endif // OTT_CLIENT_H
