#include "english_server.hpp"
#include "../../log.h"
using std::string;
using std::optional;
EnglishServer::EnglishServer(int port) {
    this->Db = new Database(this->GetDatabaseLocale());
    this->Port = port;
}

string EnglishServer::GetDatabaseLocale() {
    return this->Locale;
}

std::optional<std::string> EnglishServer::FindByName(const string& name) {
    const std::string NOT_FOUND{"Not found"};
    string record = this->Db->FindByName(name);
    if (record == NOT_FOUND) {
        return std::nullopt;
    }
    return std::optional<string>{record};
}

int EnglishServer::GetPort() {
    return this->Port;
}

EnglishServer::~EnglishServer() {
    LogInfo("Shutdowning the english server...");
    delete this->Db;
}