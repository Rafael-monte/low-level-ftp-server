#ifndef __ABSTRACT_SERVER_HPP__
#define __ABSTRACT_SERVER_HPP__
#include <string>
#include <optional>
#include <vector>
#include <exception>
#define PURE 0
using std::string;
using std::optional;

class AbstractServer {
    public:
        virtual std::optional<std::string> FindByName(const std::string& name)=PURE;
        virtual std::string GetDatabaseLocale()=PURE;
        virtual int GetPort()=PURE;
        ~AbstractServer() {};   
};


#endif