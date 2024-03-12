#include "server_factory.hpp"


AbstractServer* ServerFactory::CreateEnglishServer(const int& port) {
    return new EnglishServer(port);
}

AbstractServer* ServerFactory::CreatePortugueseServer(const int& port) {
    return new PortugueseServer(port);
}

AbstractServer* ServerFactory::CreateSpanishServer(const int& port) {
    throw new std::runtime_error("Not yet implemented!");
}