#ifndef __NAMESEARCHER_HPP__
#define __NAMESEARCHER_HPP__
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <string.h>
#include <algorithm>

const char* PT_BR_DB="pt-BR.txt";
const char* EN_US_DB="en-US.txt";

typedef struct Person {
    std::string Name;
    std::string BirthDate;
    std::string Hobbie;
    std::string Job;
} Person;

typedef enum DB {
    EN_US=0,
    PT_BR=1
} DB;

typedef std::map<char, std::vector<Person>> PersonMap;

// Configuração do locale do banco
constexpr DB SELECTED_DATABASE=DB::PT_BR;

std::string GetSelectedDatabase(const DB& selected_database) {
    switch(selected_database) {
        case DB::PT_BR:
            return std::string{PT_BR_DB};
            break;
        default:
            return std::string{EN_US_DB}; 
    }
}

PersonMap CreatePersonsDataDictionary() {
std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path filePath = currentPath / "db" / GetSelectedDatabase(SELECTED_DATABASE);

    std::ifstream inputFile(filePath);

    std::map<char, std::vector<Person>> personMap;

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return personMap;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        if (!line.empty() && line[0] != '#') {
            std::istringstream iss(line);
            Person person;
            std::getline(iss, person.Name, '|');
            std::getline(iss, person.BirthDate, '|');
            std::getline(iss, person.Hobbie, '|');
            std::getline(iss, person.Job, '|');
            char startingLetter = person.Name[0];
            personMap[startingLetter].push_back(person);
        }
    }

    inputFile.close();

    return personMap;
}

std::string PersonToString(Person& person) {
    std::string person_as_string{person.Name};
    person_as_string.append("|");
    person_as_string.append(person.BirthDate);
    person_as_string.append("|");
    person_as_string.append(person.Hobbie);
    person_as_string.append("|");
    person_as_string.append(person.Job);
    return person_as_string;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

std::string SearchByName(PersonMap& personMap, const std::string& name) {
    const std::string NOT_FOUND{"Not found"};
    std::string uppercaseName = trim(name);
    std::transform(uppercaseName.begin(), uppercaseName.end(), uppercaseName.begin(), ::toupper);
    char firstLetter = uppercaseName.empty() ? '\0' : std::toupper(uppercaseName[0]);
    auto it = personMap.find(firstLetter);
    if (it != personMap.end()) {
        for (Person& person : it->second) {
            std::string trimmedUppercasePersonName = trim(person.Name);
            std::transform(trimmedUppercasePersonName.begin(), trimmedUppercasePersonName.end(), trimmedUppercasePersonName.begin(), ::toupper);
            if (strncmp(uppercaseName.c_str(), trimmedUppercasePersonName.c_str(), trimmedUppercasePersonName.size()) == 0) {
                return PersonToString(person);
            }
        }
    }
    return NOT_FOUND;
}

std::string SelectedDatabase() {
    std::string selected_db{"Locale do banco de dados: "};
    selected_db+=GetSelectedDatabase(SELECTED_DATABASE);
    return selected_db;
}

#endif // __NAMESEARCHER_HPP__

