#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <Misc/Timer.hpp>

class ClientConfig;
class NotSofiaSession;

class ClientConnection
        : public boost::enable_shared_from_this<ClientConnection>, public Timer {
public:
    typedef boost::shared_ptr<ClientConnection> pointer;

    static pointer create(boost::asio::io_context&, const ClientConfig*);

    void start();
    void stop();

    ~ClientConnection();

    boost::signals2::signal<void(int chId)> onMotion;

private:
    ClientConnection(boost::asio::io_context&, const ClientConfig*);

    void onReconnectTick();
    void startImpl();

    std::unique_ptr<const ClientConfig> m_config;
    boost::signals2::signal<void ()> closeClients;
    bool m_stopped;
};

#endif /* CLIENT_CONNECTION_HPP */
