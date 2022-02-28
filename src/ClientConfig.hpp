#ifndef CLIENT_CONFIG_HPP
#define CLIENT_CONFIG_HPP

#include <Config/ConfigItem.hpp>

class ClientConfig : public ConfigItem {
public:
    ClientConfig(const boost::json::object&);

    const std::string& getIp() const;
    int getPort() const;
    const std::string& getComment() const;

private:
    const std::string m_ip;
    const int m_port;
    const std::string m_comment;
};

#endif /* CLIENT_CONFIG_HPP */
