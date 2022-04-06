#ifndef NOT_SOFIA_SESSION_HPP
#define NOT_SOFIA_SESSION_HPP

#include <Network/Session/Session.hpp>

class NotSofiaSession : public Session {
public:
    NotSofiaSession(Socket*);
    ~NotSofiaSession();

    boost::signals2::signal<void(int chId)> onMotion;

private:
    TAwaitVoid work() override final;

    void onPingTick();
    TAwaitVoid sendCmd(const std::string& cmd);
    void readHandler(const uint8_t *ptr, std::size_t size);
    void onCommandLine(const std::string& cmd);

    std::string m_strRecv;
};

#endif /* NOT_SOFIA_SESSION_HPP */
