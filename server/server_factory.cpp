#include "server_factory.hpp"
#include "impl/english_server.cpp"
#include "impl/portuguese_server.cpp"
#include "impl/spanish_server.cpp"

AbstractServer* ServerFactory::CreateEnglishServer(const int& port) {
    return new EnglishServer(port);
}

AbstractServer* ServerFactory::CreatePortugueseServer(const int& port) {
    return new PortugueseServer(port);
}

AbstractServer* ServerFactory::CreateSpanishServer(const int& port) {
    return new SpanishServer(port);
}

std::vector<AbstractServer*> ServerFactory::CreateAllServers(const int& startPort) {
    std::vector<AbstractServer*> servers{
        this->CreateEnglishServer(startPort), 
        this->CreatePortugueseServer(startPort+1),
        this->CreateSpanishServer(startPort+2)
    };
    return servers;
}