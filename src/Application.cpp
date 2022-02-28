#include "Application.h"
#include "ClientConnection.hpp"
#include "Config/ClientConfig.hpp"
#include "Config/NotSofiaConfig.hpp"

#include <IPC/IntQueue.h>
#include <Misc/StrandHolder.hpp>
#include <Misc/Debug.hpp>

#define PROG_NAME       "notSofiaAlarmClient"
#define IPC_QUEUE_NAME  "shinobi_alarm_queue"

Application::Application(int argc, char* argv[])
    : BaseConfigApplication(PROG_NAME, argc, argv) {
}

Application::~Application() {
}

Config* Application::createConfig() {
    return new NotSofiaConfig();
}

void Application::doExit() {
    m_closeConnections();
}

void Application::start(TConfigItems &notSofiaServers) {
    m_strand.reset(new StrandHolder(io()));
    m_intQueue.reset(new IPCIntQueue(IPC_QUEUE_NAME));

    for (auto& config : notSofiaServers) {
        auto c = ClientConnection::create(io(), static_cast<const ClientConfig*>(config.release()));

        c->onMotion.connect([this](int chId) {
            m_strand->post([this, chId] {
                m_intQueue->send(chId);
                //debug_print(boost::format("onMotion %1% %2%") % this % chId);
            });
        });

        m_closeConnections.connect_extended([c](const boost::signals2::connection& con) {
            con.disconnect();
            c->stop();
        });

        c->start();
    }
}
