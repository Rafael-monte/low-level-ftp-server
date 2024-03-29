#ifndef __ENGLISH_SERVER_HPP__
#define __ENGLISH_SERVER_HPP__
#include "../abstract_server.hpp"
#include "../../db/database_connector.hpp"
#include <string>
#include <optional>

class EnglishServer: public AbstractServer {
    private:
        Database* Db;
        std::string Locale{"en-US"};
        int Port;
    public:
        EnglishServer(int port);
        int GetPort() override;
        std::string GetDatabaseLocale() override;
        Database* GetDatabase() override;
        ~EnglishServer();
};


#endif