#include "portuguese_server.hpp"
#include "../../log.h"
using std::string;
using std::optional;
PortugueseServer::PortugueseServer(int port) {
    LogInfo("Criando Servidor Brasileiro...");
    LogInfo("Criando Banco de dados...");
    this->Db = new Database(this->GetDatabaseLocale());
    this->Port = port;
    LogSuccess("Servidor Brasileiro criado com sucesso");
}

string PortugueseServer::GetDatabaseLocale() {
    return this->Locale;
}

optional<string> PortugueseServer::FindByName(const string& name) {
    std::string notFound = "Não foi possível encontrar a string \""+name+"\" no servidor brasileiro.";
    const std::string NOT_FOUND{"Not found"};
    string record = this->Db->FindByName(name);
    if (record == NOT_FOUND) {
        LogWarning(notFound.c_str());
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