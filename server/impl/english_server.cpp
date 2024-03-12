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

std::optional<std::string> EnglishServer::FindByName(const string& name) {
    std::string notFound="Couldn't find the string \""+name+"\" in the english server.";
    const std::string NOT_FOUND{"Not found"};
    string record = this->Db->FindByName(name);
    if (record == NOT_FOUND) {
        LogWarning(notFound.c_str());
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