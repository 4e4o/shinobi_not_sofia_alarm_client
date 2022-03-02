#include "ClientConfig.hpp"

#include <boost/json.hpp>

ClientConfig::ClientConfig(const boost::json::object &o)
    : m_ip(get<std::string>(o, "ip")),
      m_port(get<std::int64_t>(o, "port")),
      m_comment(get<std::string, true>(o, "comment")) {
}

const std::string& ClientConfig::getIp() const {
    return m_ip;
}

int ClientConfig::getPort() const {
    return m_port;
}

const std::string& ClientConfig::getComment() const {
    return m_comment;
}
