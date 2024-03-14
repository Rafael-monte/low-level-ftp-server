#include "english_server.hpp"
#include "../../log.h"
using std::string;
using std::optional;
EnglishServer::EnglishServer(int port) {
    LogInfo("Creating english server...");
    LogInfo("Creating Database...");
    this->Db = new Database(this->GetDatabaseLocale());
    this->Port = port;
    LogSuccess("English server created successfully");
}

string EnglishServer::GetDatabaseLocale() {
    return this->Locale;
}

int EnglishServer::GetPort() {
    return this->Port;
}

Database* EnglishServer::GetDatabase() {
    return this->Db;
}

EnglishServer::~EnglishServer() {
    LogInfo("Shutdowning the english server...");
    delete this->Db;
}