#ifndef NOT_SOFIA_CONFIG_HPP
#define NOT_SOFIA_CONFIG_HPP

#include <Config/Config.hpp>

class NotSofiaConfig : public Config {
public:
    NotSofiaConfig();
    ~NotSofiaConfig();

private:
    ConfigItem* parseItem(const boost::json::object&) override final;
};

#endif /* NOT_SOFIA_CONFIG_HPP */
