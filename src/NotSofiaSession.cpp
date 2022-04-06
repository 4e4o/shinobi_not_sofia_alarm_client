#include "NotSofiaSession.hpp"

#include <Network/Session/Operations/SessionReader.hpp>
#include <Network/Session/Operations/SessionWriter.hpp>
#include <Misc/ScopeGuard.hpp>
#include <Misc/Debug.hpp>

using namespace std::literals::chrono_literals;

#define PING_INTERVAL       4s
#define RECEIVE_TIMEOUT     15s

#define CMD_SEPARATOR       "\n"

using namespace boost::asio;

NotSofiaSession::NotSofiaSession(Socket *s)
    : Session(s) {
}

NotSofiaSession::~NotSofiaSession() {
}

TAwaitVoid NotSofiaSession::sendCmd(const std::string& cmd) {
    const std::string send = cmd + CMD_SEPARATOR;
    std::vector<uint8_t> data(send.begin(), send.end());
    co_await writer().all(data);
    co_return;
}

TAwaitVoid NotSofiaSession::work() {
    auto pinger = scopedSpawn<true>([this]() -> TAwaitVoid {
        while(running()) {
            co_await sendCmd("PI");
            co_await wait(PING_INTERVAL);
        }
    });

    while(running()) {
        std::size_t size = co_await timeout(reader().some(), RECEIVE_TIMEOUT);
        readHandler(reader().ptr(), size);
    }
}

void NotSofiaSession::readHandler(const uint8_t *ptr, std::size_t size) {
    m_strRecv.append((char*)ptr, size);

    size_t index;
    std::string line;

    while ((index = m_strRecv.find(CMD_SEPARATOR)) != std::string::npos) {
        line = m_strRecv.substr(0, index);
        m_strRecv.erase(0, index + strlen(CMD_SEPARATOR));
        onCommandLine(line);
    }
}

void NotSofiaSession::onCommandLine(const std::string& cmd) {
    debug_print_this(fmt("%1%") % cmd);

    if (cmd == "PO") {
        return;
    } else if (cmd.starts_with("MD ")) {
        const int chId = std::stoi(cmd.substr(3));
        onMotion(chId);
        return;
    }

    throw std::runtime_error("unknown command");
}
