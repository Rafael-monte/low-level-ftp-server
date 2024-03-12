#include "server_factory.hpp"
#include "impl/english_server.cpp"
#include "impl/portuguese_server.cpp"

AbstractServer* ServerFactory::CreateEnglishServer(const int& port) {
    return new EnglishServer(port);
}

AbstractServer* ServerFactory::CreatePortugueseServer(const int& port) {
    return new PortugueseServer(port);
}

AbstractServer* ServerFactory::CreateSpanishServer(const int& port) {
    auto p = port;
    std::string notImplemented{"Not yet implemented! In port"};
    notImplemented+=p;
    throw new std::runtime_error(notImplemented);
}