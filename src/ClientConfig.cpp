#include "ClientConfig.hpp"

#include <boost/json.hpp>

ClientConfig::ClientConfig(const boost::json::object &o)
    : m_ip(toStdString(o.at("ip").as_string())),
      m_port(o.at("port").as_int64()),
      m_comment(o.contains("comment") ? toStdString(o.at("comment").as_string()) : "") {
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
