#ifndef NOT_SOFIA_SESSION_HPP
#define NOT_SOFIA_SESSION_HPP

#include <Network/Session.hpp>

class Timer;
class ClientConfig;
class QueuedSessionWriter;

class NotSofiaSession : public Session {
public:
    NotSofiaSession(const ClientConfig*, boost::asio::io_context &io);
    NotSofiaSession(const ClientConfig*, boost::asio::io_context &io, TCPSocket&& sock);
    ~NotSofiaSession();

    boost::signals2::signal<void(int chId)> onMotion;

private:
    void startImpl() override final;

    void onPingTick();
    void onActivityTick();
    void sendCmd(const std::string& cmd);
    void readHandler(const uint8_t *ptr, std::size_t size);
    bool onCommandLine(const std::string& cmd);
    void readCmd();

    const ClientConfig* m_config;
    std::unique_ptr<QueuedSessionWriter> m_writer;
    std::unique_ptr<Timer> m_pingTimer;
    std::unique_ptr<Timer> m_activityTimer;
    std::string m_strRecv;
    int m_recvCount;
};

#endif /* NOT_SOFIA_SESSION_HPP */
