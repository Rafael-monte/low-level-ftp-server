#ifndef __SERVER_FACTORY_HPP__
#define __SERVER_FACTORY_HPP__
#include <vector>
#include "abstract_server.hpp"
#include "impl/english_server.hpp"
#include "impl/portuguese_server.hpp"
#include "impl/spanish_server.hpp"

class ServerFactory {
    public:
        AbstractServer* CreatePortugueseServer(const int& port);
        AbstractServer* CreateEnglishServer(const int& port);
        AbstractServer* CreateSpanishServer(const int& port);
        std::vector<AbstractServer*> CreateAllServers(const int& startPort);

};



#endif