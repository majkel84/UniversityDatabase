#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Person.hpp"

using personIter = std::vector<std::shared_ptr<Person>>::iterator;
using personPtr = std::shared_ptr<Person>;

class Database
{
    private:
        std::vector<personPtr> data_ {};
        constexpr static auto dbFilename = "../database.txt"; //literal
        bool parseLineByLine(std::ifstream& ifs);
        bool writeLineByLine(std::ofstream& ofs);
    public:
        personIter searchByLastName(const std::string& lastName);
        personIter searchByPersonalID(const unsigned long long& personalID);
        personIter searchByStudentID(const unsigned long& studentID);
        void printDatabase() const;
        void sortBySalary();
        void sortByLastName();
        void sortByPersonalID();
        void sortByStudentID();
        void addPerson(personPtr person);

        bool addStudent(const std::string& firstName,
                const std::string& lastName,
                const unsigned long long& personalID,
                const Gender& gender,
                const std::string& address,
                const unsigned long& studentIndex);

        bool addEmployee(const std::string& firstName,
                const std::string& lastName,
                const unsigned long long& personalID,
                const Gender& gender,
                const std::string& address,
                const double& salary);

        bool loadFromFile(const std::string filename = dbFilename);
        bool saveToFile(const std::string filename = dbFilename);
        bool removeByPersonalID(const unsigned long long& personalID);
        bool removeByStudentID(const unsigned long& studentID);
        bool modifySalary(const unsigned long long& personalID, const double& newSalary);
        bool modifyAddress(const unsigned long long& personalID, const std::string& address);
};
