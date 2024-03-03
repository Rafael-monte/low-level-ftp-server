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

typedef struct Person {
    std::string Name;
    std::string BirthDate;
    std::string Hobbie;
    std::string Job;
} Person;

typedef std::map<char, std::vector<Person>> PersonMap;

PersonMap CreatePersonsDataDictionary() {
std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path filePath = currentPath / "db" / "persons.txt";

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

#endif // __NAMESEARCHER_HPP__

