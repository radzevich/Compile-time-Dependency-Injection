#pragma once

#include "../domain//company.h"
#include "../domain/employee.h"
#include "dependency_exposure.h"
//#include "bootstrap.h"
#include <concepts>
#include <string>

template <class TEmployeeRepository>
concept EmployeeRepository = requires(TEmployeeRepository& repository, Employee employee) {
    { repository.Get(employee.Id) } -> std::same_as<Employee>;
    { repository.Add(employee) } -> std::same_as<decltype(employee.Id)>;
    { repository.Update(employee) };
    { repository.Remove(employee.Id) };
};

template <class TCompanyRepository>
concept CompanyRepository = requires(TCompanyRepository& repository, Company company) {
    { repository.Get(company.Id) } -> std::same_as<Company>;
    { repository.Add(company) } -> std::same_as<decltype(company.Id)>;
    { repository.Update(company) };
    { repository.Remove(company.Id) };
};

template <EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
class UnitOfWork {
private:
    TEmployeeRepository* EmployeeRepository_;
    TCompanyRepository* CompanyRepository_;

public:
    UnitOfWork(TEmployeeRepository* employeeRepository, TCompanyRepository* companyRepository);

    void AddCompany(const std::string& companyName);
    int AddEmployee(int companyId, const std::string& employeeName);
    void RotateEmployee(int fromCompanyId, int toCompanyId, int employeeId);
    void RemoveEmployee(int companyId, int employeeId);
};

