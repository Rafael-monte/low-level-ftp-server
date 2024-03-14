#include "spanish_server.hpp"
#include "../../log.h"
using std::string;
using std::optional;
SpanishServer::SpanishServer(int port) {
    LogInfo("Creando servidor Español...");
    LogInfo("Creando Base de datos...");
    this->Db = new Database(this->GetDatabaseLocale());
    this->Port = port;
    LogSuccess("Servidor Español creado con éxito");
}

string SpanishServer::GetDatabaseLocale() {
    return this->Locale;
}

int SpanishServer::GetPort() {
    return this->Port;
}

Database* SpanishServer::GetDatabase() {
    return this->Db;
}

SpanishServer::~SpanishServer() {
    LogInfo("Apagando a el servidor Español...");
    delete this->Db;
}