#pragma once

#include "../domain//company.h"
#include "../domain/employee.h"

#include <concepts>

template <class TEmployeeRepository>
concept EmployeeRepository = requires(TEmployeeRepository& repository, Employee employee) {
    { repository.GetEmployee() } -> std::same_as<Employee>;
    { repository.SaveEmployee(employee) };
};

template <class TCompanyRepository>
concept CompanyRepository = requires(TCompanyRepository& repository, Company company) {
    { repository.GetCompany() } -> std::same_as<Company>;
    { repository.SaveCompany(company) };
};

template <EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
class AccountService {
private:
    TEmployeeRepository& EmployeeRepository_;
    TCompanyRepository& CompanyRepository_;
public:
    AccountService(TEmployeeRepository& employeeRepository, TCompanyRepository& companyRepository)
        : EmployeeRepository_(employeeRepository)
        , CompanyRepository_(companyRepository) {
    }
};
