#include "NotSofiaConfig.hpp"
#include "ClientConfig.hpp"

#include <boost/json/src.hpp>

NotSofiaConfig::NotSofiaConfig()
    : Config("clients") {
    registerDefaultType<ClientConfig, Config*, const boost::json::object&>();
}

NotSofiaConfig::~NotSofiaConfig() {
}

ConfigItem* NotSofiaConfig::parseItem(const boost::json::object& obj) {
    return create<ClientConfig>(static_cast<Config*>(this), obj);
}
