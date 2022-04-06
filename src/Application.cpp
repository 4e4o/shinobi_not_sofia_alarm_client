#include "Application.h"
#include "NotSofiaSession.hpp"
#include "ClientConfig.hpp"

#include <Config/Config.hpp>
#include <Network/Client.hpp>
#include <Misc/StrandHolder.hpp>
#include <Misc/Debug.hpp>
#include <IPC/IntQueue.h>

using namespace std::literals::chrono_literals;

#define RECONNECT_INTERVAL  5s

#define IPC_QUEUE_NAME      "shinobi_alarm_queue"

Application::Application(int argc, char* argv[])
    : BaseConfigApplication(argc, argv) {
    config()->registerType<ConfigItem, ClientConfig, const boost::json::object&>();
}

Application::~Application() {
}

void Application::doExit() {
    m_close();
}

bool Application::start(TConfigItems &notSofiaServers) {
    m_strand.reset(new StrandHolder(io()));
    m_intQueue.reset(new IPCIntQueue(IPC_QUEUE_NAME));

    for (auto& config : notSofiaServers) {
        const ClientConfig* clientConfig = static_cast<const ClientConfig*>(config.get());
        TClient client(new Client(io(), RECONNECT_INTERVAL));
        client->registerType<Session, NotSofiaSession, Socket*>();

        client->newSession.connect([this](TWSession ws) {
            auto session = std::static_pointer_cast<NotSofiaSession>(ws.lock());
            session->onMotion.connect([this](int chId) {
                m_strand->post([this, chId] {
                    m_intQueue->send(chId);
                    debug_print_this(fmt("onMotion %1%") % chId);
                });
            });
        });

        Lifecycle::connectTrack(m_close, client, &Client::stop);
        client->start(clientConfig->getIp(), clientConfig->getPort());
    }

    return true;
}
