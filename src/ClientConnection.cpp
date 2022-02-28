#include <Network/Client.hpp>

#include "NotSofiaSession.hpp"
#include "ClientConnection.hpp"
#include "Config/ClientConfig.hpp"

#include <Misc/Debug.hpp>

#define RECONNECT_INTERVAL_SEC  5

using boost::signals2::connection;

ClientConnection::pointer ClientConnection::create(boost::asio::io_context& io_context, const ClientConfig *c) {
    return pointer(new ClientConnection(io_context, c));
}

ClientConnection::ClientConnection(boost::asio::io_context& io, const ClientConfig *c)
    : Timer(io, RECONNECT_INTERVAL_SEC), m_config(c), m_stopped(false) {
    debug_print(boost::format("ClientConnection::ClientConnection %1%") % this);
    onTimeout.connect([this](Timer*) {
        onReconnectTick();
    });
}

ClientConnection::~ClientConnection() {
    debug_print(boost::format("ClientConnection::~ClientConnection %1%") % this);
}

void ClientConnection::onReconnectTick() {
    if (m_stopped)
        return;

    start();
}

void ClientConnection::start() {
    auto self = shared_from_this();
    post([self] {
        self->startImpl();
    });
}

void ClientConnection::startImpl() {
    std::shared_ptr<NotSofiaSession> session(new NotSofiaSession(m_config.get(), io()));
    std::shared_ptr<Client> client(new Client(io()));
    client->setSession(session);

    session->setStrand(this, false);
    client->setStrand(this, false);

    session->onMotion.connect([this](int chId) {
        STRAND_ASSERT(this);
        onMotion(chId);
    });

    closeClients.connect(decltype(closeClients)::slot_type(
                             &NotSofiaSession::close, session.get()).track_foreign(session));

    auto self = shared_from_this();

    session->onDestroy.connect_extended([self](const connection &c) {
        c.disconnect();

        self->post([self] {
            if (self->m_stopped)
                return;

            self->startTimer();
        });
    });

    client->onConnect.connect_extended([self, session](const connection &c, bool connected) {
        c.disconnect();
        STRAND_ASSERT(self.get());

        if (self->m_stopped)
            return;

        if (connected) {
            session->start();
        }
    });

    client->connect(m_config->getIp(), m_config->getPort());
}

void ClientConnection::stop() {
    auto self = shared_from_this();
    post([self] {
        if (self->m_stopped)
            return;

        self->m_stopped = true;
        self->closeClients();
        self->stopTimer();
    });
}
