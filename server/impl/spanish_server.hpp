#ifndef __SPANISH_SERVER_HPP__
#define __SPANISH_SERVER_HPP__
#include "../abstract_server.hpp"
#include "../../db/database_connector.hpp"
#include <string>
#include <optional>

class SpanishServer: public AbstractServer {
    private:
        Database* Db;
        std::string Locale{"es-ES"};
        int Port;
    public:
        SpanishServer(int port);
        int GetPort() override;
        std::string GetDatabaseLocale() override;
        Database* GetDatabase() override;
        ~SpanishServer();
};

#endif