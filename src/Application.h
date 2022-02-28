#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <BaseConfigApplication.h>
#include <boost/signals2.hpp>

class IPCIntQueue;
class StrandHolder;

class Application : public BaseConfigApplication {
public:
    Application(int argc, char* argv[]);
    ~Application();

private:
    Config* createConfig() override final;
    void start(TConfigItems&) override final;
    void doExit() override final;

    std::unique_ptr<StrandHolder> m_strand;
    std::unique_ptr<IPCIntQueue> m_intQueue;
    boost::signals2::signal<void()> m_closeConnections;
};

#endif /* APPLICATION_HPP */
