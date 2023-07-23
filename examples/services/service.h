#pragma once

#include "../domain//company.h"
#include "../domain/employee.h"

#include <concepts>

template <class TEmployeeRepository>
concept EmployeeRepository = requires(TEmployeeRepository& repository, Example::Domain::Employee employee) {
    { repository.GetEmployee() } -> std::same_as<Example::Domain::Employee>;
    { repository.SaveEmployee(employee) };
};

template <class TCompanyRepository>
concept CompanyRepository = requires(TCompanyRepository& repository, Example::Domain::Company company) {
    { repository.GetCompany() } -> std::same_as<Example::Domain::Company>;
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
