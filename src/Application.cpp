#include "Application.h"
#include "NotSofiaSession.hpp"
#include "ClientConfig.hpp"

#include <Config/Config.hpp>
#include <Network/ClientManager.hpp>
#include <Misc/StrandHolder.hpp>
#include <Misc/Debug.hpp>
#include <IPC/IntQueue.h>

#define PROG_NAME       "notSofiaAlarmClient"
#define IPC_QUEUE_NAME  "shinobi_alarm_queue"
#define RECONNECT_SEC   5

Application::Application(int argc, char* argv[])
    : BaseConfigApplication(PROG_NAME, argc, argv) {
    config()->registerType<ConfigItem, ClientConfig, const boost::json::object&>();
}

Application::~Application() {
}

void Application::doExit() {
    m_closeConnections();
}

void Application::start(TConfigItems &notSofiaServers) {
    m_strand.reset(new StrandHolder(io()));
    m_intQueue.reset(new IPCIntQueue(IPC_QUEUE_NAME));

    for (auto& config : notSofiaServers) {
        const ClientConfig* clientConfig = static_cast<const ClientConfig*>(config.get());
        std::shared_ptr<ClientManager> client(new ClientManager(io(), RECONNECT_SEC));
        client->registerType<Session, NotSofiaSession, boost::asio::io_context&>();

        client->onNewSession.connect([this](Session *s) {
            NotSofiaSession *session = static_cast<NotSofiaSession*>(s);
            session->onMotion.connect([this](int chId) {
                m_strand->post([this, chId] {
                    m_intQueue->send(chId);
                    debug_print(boost::format("onMotion %1% %2%") % this % chId);
                });
            });
        });

        m_closeConnections.connect_extended([client](const boost::signals2::connection& con) {
            con.disconnect();
            client->stop();
        });

        client->start(clientConfig->getIp(), clientConfig->getPort());
    }
}
