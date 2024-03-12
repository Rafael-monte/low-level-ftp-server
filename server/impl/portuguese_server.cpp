#include "portuguese_server.hpp"
#include "../../log.h"
using std::string;
using std::optional;
PortugueseServer::PortugueseServer(int port) {
    this->Db = new Database(this->GetDatabaseLocale());
    this->Port = port;
}

string PortugueseServer::GetDatabaseLocale() {
    return this->Locale;
}

optional<string> PortugueseServer::FindByName(const string& name) {
    const std::string NOT_FOUND{"Not found"};
    string record = this->Db->FindByName(name);
    if (record == NOT_FOUND) {
        return std::nullopt;
    }
    return optional<string>{record};
}

int PortugueseServer::GetPort() {
    return this->Port;
}

PortugueseServer::~PortugueseServer() {
    LogInfo("Desligando o servidor brasileiro...");
    delete this->Db;
}