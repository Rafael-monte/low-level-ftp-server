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

int PortugueseServer::GetPort() {
    return this->Port;
}

Database* PortugueseServer::GetDatabase() {
    return this->Db;
}

PortugueseServer::~PortugueseServer() {
    LogInfo("Desligando o servidor brasileiro...");
    delete this->Db;
}