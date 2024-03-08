#include "database_connector.hpp"
#include "../log.h"

Database::Database(char** argv, int argc) {
    const vector<string> LOCALES{"en-US", "pt-BR"};
    vector<string> arguments{argv, argv + argc};
    
    if (arguments.size() != 2) {
        LogError("Número de argumentos incorretos. Insira apenas o nome do servidor e o locale ('pt-BR' ou 'en-US')");
        exit(EXIT_FAILURE);
    }

    const auto selectedLocale=arguments[1];
    if (std::find(LOCALES.begin(), LOCALES.end(), selectedLocale) != LOCALES.end()) {
        string findMessage{"Banco de dados selecionado: "};
        findMessage+=selectedLocale;
        this->SelectedSchema=selectedLocale;
        LogInfo(findMessage.c_str());
    } else {
        LogWarning("Não foi possível encontrar o banco de dados indicado, utilizando o banco de dados padrão.");
        this->SelectedSchema=this->DEFAULT_SCHEMA;
    }
    try {
        this->Data=CreatePersonsDataDictionary();
    } catch (std::exception* e) {
        LogError("Não foi possível construir banco de dados");
        exit(EXIT_FAILURE);
    }
}

string Database::GetSelectedSchema() {
    const string EXTENSION{".txt"};
    return this->SelectedSchema+EXTENSION;
}

PersonMap Database::CreatePersonsDataDictionary() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path filePath = currentPath / "db" / GetSelectedSchema();

    std::ifstream inputFile(filePath);

    std::map<char, std::vector<Person>> personMap;

    if (!inputFile.is_open()) {
        LogWarning("Não foi possível abrir o banco de dados");
        throw std::runtime_error("Não foi possível abrir banco de dados");
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

std::string Database::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

std::string Database::PersonToString(Person& person) {
    std::string person_as_string{person.Name};
    person_as_string.append("|");
    person_as_string.append(person.BirthDate);
    person_as_string.append("|");
    person_as_string.append(person.Hobbie);
    person_as_string.append("|");
    person_as_string.append(person.Job);
    return person_as_string;
}

std::string Database::FindByName(const std::string& name) {
    auto personMap=this->Data;
    const std::string NOT_FOUND{"Not found"};
    std::string uppercaseName = this->trim(name);
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