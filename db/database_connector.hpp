#ifndef __DATABASE_CONNECTOR_HPP__
#define __DATABASE_CONNECTOR_HPP__
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <string.h>
#include <algorithm>
#include <map>
#include <exception>
using std::string;
using std::vector;
using std::map;

typedef struct Person {
    std::string Name;
    std::string BirthDate;
    std::string Hobbie;
    std::string Job;
} Person;

typedef map<char,vector<Person>> PersonMap;
class Database {
    private:
        string SelectedSchema{""};
        const char* DEFAULT_SCHEMA="en-US";
        PersonMap Data;
        bool LowLevelComparison(const string& str, const string& another_str);
        PersonMap CreatePersonsDataDictionary();
        string trim(const string& str);
        string PersonToString(Person& personMap);
    public:
        Database(char** argv, int argc);
        Database(const string& locale);
        string GetSelectedSchema();
        string FindByName(const string& name);
};


#endif
