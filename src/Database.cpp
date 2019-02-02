#include "Database.hpp"
#include "Employee.hpp"
#include "Student.hpp"
#include <fstream>
#include <algorithm>

using studentPtr = std::shared_ptr<Student>;
using employeePtr = std::shared_ptr<Employee>;
using std::dynamic_pointer_cast;

personIter Database::searchByLastName(const std::string& lastName)
{
    auto it = std::find_if(begin(data), end(data), [lastName] (personPtr person)
            {
            return person -> getLastName() == lastName;
            });

    if (it != end(data))
        std::cout << *it << "\n";
    else
        std::cout << "Person " << lastName << " not found\n\n";
    return it;
}

personIter Database::searchByPersonalID(const unsigned long long& personalID)
{
    auto it = std::find_if(begin(data), end(data), [personalID] (personPtr person)
            {
            return person -> getPersonalID() == personalID;
            });

    if (it != end(data))
        std::cout << *it << "\n";
    else
        std::cout << "Personal ID " << personalID << " not found.\n\n";
    return it;
}

personIter Database::searchByStudentID(const unsigned long& studentID)
{
    auto it = std::find_if(begin(data), end(data), [studentID] (personPtr person)
            {
            studentPtr student = dynamic_pointer_cast<Student>(person);
            return student -> getStudentIndex() == studentID;
            });

    if (it != end(data))
        std::cout << *it << "\n";
    else
        std::cout << "Student ID " << studentID << " not found.\n\n";
    return it;
}

void Database::printDatabase() const
{
    if(data.empty())
        std::cout << "Empty database!\n";

    for(const auto& personPtr : data)
        std::cout << personPtr;
    std::cout << "\n";
}

void Database::sortBySalary()
{
    std::sort(begin(data), end(data), [](personPtr left, personPtr right)
            {
            //input: 10.1 NaN 2.5  NaN 3.6
            //output: 2.5 3.6 10.1 Nan Nan

            //NaN 2.5 --- if student on the left: bad!
            if(dynamic_pointer_cast<Student>(left)) return false;

            //2.5 NaN -- if student on the right: good! move all to the right
            if(dynamic_pointer_cast<Student>(right)) return true;

            employeePtr e_left = dynamic_pointer_cast<Employee>(left);
            employeePtr e_right = dynamic_pointer_cast<Employee>(right);
            //if employee, compare it usually
            return e_left->getSalary() < e_right->getSalary();
            });

}

void Database::sortByLastName()
{
    std::sort(begin(data), end(data), [](personPtr left, personPtr right)
            {
            return left->getLastName() < right->getLastName();
            });
}

void Database::sortByPersonalID()
{
    std::sort(begin(data), end(data), [](personPtr left, personPtr right)
            {
            return left->getPersonalID() < right->getPersonalID();
            });
}

void Database::sortByStudentID()
{
    std::sort(begin(data), end(data), [](personPtr left, personPtr right)
            {
            if(dynamic_pointer_cast<Employee>(left)) return false;
            if(dynamic_pointer_cast<Employee>(right)) return true;

            studentPtr student1 = dynamic_pointer_cast<Student>(left);
            studentPtr student2 = dynamic_pointer_cast<Student>(right);
            return student1->getStudentIndex() < student2->getStudentIndex();
            });
}

void Database::addPerson(personPtr person)
{
    data.push_back(person);
}

bool Database::addStudent(const std::string& firstName,
        const std::string& lastName,
        const unsigned long long& personalID,
        const bool& gender,
        const std::string& address,
        const unsigned long& studentIndex)
{
    personPtr student = std::make_shared<Student>(firstName,
            lastName, personalID, gender, address, studentIndex);
    addPerson(student);
    return true;
}

bool Database::addEmployee(const std::string& firstName,
        const std::string& lastName,
        const unsigned long long& personalID,
        const bool& gender,
        const std::string& address,
        const double& salary)
{
    personPtr employee = std::make_shared<Employee>(firstName,
            lastName, personalID, gender, address, salary);
    addPerson(employee);
    return true;
}


bool Database::loadFromFile(const std::string filename/*="database.txt"*/)
{
    std::ifstream ifs {filename}; //input file stream
    if(!ifs){
        std::cout << "Could not open " << filename << " for reading!\n";
        return false;
    }

    std::string firstName, lastName, address, salary, studentIndex;
    unsigned long long personalID;
    bool gender;

    while(ifs >> lastName >> firstName >> personalID >> gender >>
            address >> salary >> studentIndex)
    {
        if(salary == "----")
            addStudent(firstName, lastName, personalID, gender, address, std::stold(studentIndex));
        else if(studentIndex == "----")
            addEmployee(firstName, lastName, personalID, gender, address, std::stod(salary));
        else{
            std::cout << "\nInvalid line in input file!\n";
            return false;
        }
    }

    return true;
}

bool Database::saveToFile(const std::string filename/*="database.txt"*/)
{
    std::ofstream ofs {filename}; //output file stream

    if(!ofs){
        std::cout << "Could not open " << filename << " for writing!\n";
        return false;
    }
    for(const auto& personPtr : data)
        ofs << personPtr;
    ofs << "\n";
    return true;
}



bool Database::removeByPersonalID(const unsigned long long& personalID)
{
    auto iter = searchByPersonalID(personalID);

    if (iter != end(data))
    {
        data.erase(iter);
        return true;
    }
    else
        return false;
}

bool Database::removeByStudentID(const unsigned long& studentID)
{
    auto iter = searchByStudentID(studentID);

    if (iter != end(data))
    {
        data.erase(iter);
        return true;
    }
    else
        return false;
}

bool Database::modifySalary(const unsigned long long& personalID, const double& newSalary)
{
    auto personIter = searchByPersonalID(personalID);

    if (personIter != data.end())
    {
        if(employeePtr isEmployee = dynamic_pointer_cast<Employee>(*personIter))
        {
            isEmployee->setSalary(newSalary);
            return true;
        }
        std::cout << "Personal ID " << personalID << " is not an Employee.\n\n";
    }
    return false;
}


bool Database::modifyAddress(const unsigned long long& personalID, const std::string& newAddress)
{
    auto personIter = searchByPersonalID(personalID);

    if (personIter != data.end())
    {    
        (*personIter)->setAddress(newAddress);
        return true;
    }
    return false;
}
