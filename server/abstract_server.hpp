#ifndef __ABSTRACT_SERVER_HPP__
#define __ABSTRACT_SERVER_HPP__
#include <string>
#include <optional>
#include <vector>
#include <exception>
#include "../db/database_connector.hpp"
#define PURE 0
using std::string;
using std::optional;

class AbstractServer {
    public:
        // Template method
        virtual std::optional<std::string> FindByName(const std::string& name) final {
            const std::string NOT_FOUND{"Not found"};
            auto record = GetDatabase()->FindByName(name);
            if (record == NOT_FOUND) {
                return std::nullopt;
            }
            return std::optional{record};
        }
        virtual Database* GetDatabase()=PURE;
        virtual std::string GetDatabaseLocale()=PURE;
        virtual int GetPort()=PURE;
        virtual ~AbstractServer()=default;   
};
#endif