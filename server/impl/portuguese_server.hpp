#ifndef __PORTUGUESE_SERVER_HPP__
#define __PORTUGUESE_SERVER_HPP__
#include "../abstract_server.hpp"
#include "../../db/database_connector.hpp"
#include <string>
#include <optional>

class PortugueseServer: public AbstractServer {
    private:
        Database* Db;
        std::string Locale{"pt-BR"};
        int Port;
    public:
        PortugueseServer(int port);
        int GetPort() override;
        std::string GetDatabaseLocale() override;
        Database* GetDatabase() override;
        ~PortugueseServer();
};


#endif