#include "NotSofiaSession.hpp"

#include <Network/QueuedSessionWriter.hpp>
#include <Misc/Timer.hpp>
#include <Misc/Debug.hpp>

#define PING_INTERVAL_SEC       4
#define RECEIVE_TIMEOUT_SEC     15
#define CMD_SEPARATOR           "\n"

using boost::signals2::connection;

NotSofiaSession::NotSofiaSession(boost::asio::io_context &io)
    : Session(io) {
    onData.connect([this](const uint8_t *ptr, std::size_t size) {
        readHandler(ptr, size);
    });
}

NotSofiaSession::~NotSofiaSession() {
}

void NotSofiaSession::startImpl() {
    setReceiveTimeout(RECEIVE_TIMEOUT_SEC);

    m_writer.reset(new QueuedSessionWriter(this));

    m_pingTimer.reset(new Timer(io(), PING_INTERVAL_SEC));
    m_pingTimer->setStrand(this, false);
    m_pingTimer->onTimeout.connect([this](Timer*) {
        onPingTick();
    });

    m_pingTimer->startTimer();

    readCmd();
}

void NotSofiaSession::readCmd() {
    readSome();
}

void NotSofiaSession::sendCmd(const std::string& cmd) {
    const std::string send = cmd + CMD_SEPARATOR;
    m_writer->writeAll(reinterpret_cast<const uint8_t *>(send.data()), send.size());
}

void NotSofiaSession::onPingTick() {
    sendCmd("PI");
}

void NotSofiaSession::readHandler(const uint8_t *ptr, std::size_t size) {
    m_strRecv.append((char*)ptr, size);

    size_t index;
    std::string line;

    while ((index = m_strRecv.find(CMD_SEPARATOR)) != std::string::npos) {
        line = m_strRecv.substr(0, index);
        m_strRecv.erase(0, index + strlen(CMD_SEPARATOR));
        if (!onCommandLine(line)) {
            close();
            return;
        }
    }

    readCmd();
}

bool NotSofiaSession::onCommandLine(const std::string& cmd) {
    //    debug_print(boost::format("NotSofiaSession::onCommandLine %1% %2%") % this % cmd);

    if (cmd == "PO") {
        m_pingTimer->startTimer();
        return true;
    }

    if (cmd.starts_with("MD ")) {
        const int chId = std::stoi(cmd.substr(3));
        onMotion(chId);
    }

    return true;
}
