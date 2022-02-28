#ifndef CLIENT_CONFIG_HPP
#define CLIENT_CONFIG_HPP

#include <Config/ConfigItem.hpp>

class Config;

class ClientConfig : public ConfigItem {
public:
    ClientConfig(Config*, const boost::json::object&);

    const std::string& getIp() const;
    int getPort() const;
    const std::string& getComment() const;

private:
    std::string m_ip;
    int m_port;
    std::string m_comment;
};

#endif /* CLIENT_CONFIG_HPP */
