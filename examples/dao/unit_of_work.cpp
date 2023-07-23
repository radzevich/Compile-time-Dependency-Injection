#include "unit_of_work.h"

#include <algorithm>

namespace Example::Dao {

    using namespace Example::Domain;
    using namespace Example::Util;

    template<EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
    int UnitOfWork<TEmployeeRepository, TCompanyRepository>::AddCompany(const std::string &companyName) {
        return CompanyRepository_->Add({
            .Name = companyName,
        });
    }

    template<EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
    int
    UnitOfWork<TEmployeeRepository, TCompanyRepository>::AddEmployee(int companyId, const std::string &employeeName) {
        // create employee
        auto employeeId = EmployeeRepository_->Add({
            .CompanyId = companyId,
            .Name = employeeName,
       });

        // add employee to company
        auto company = CompanyRepository_->Get(companyId);
        company.Employees.push_back(employeeId);
        CompanyRepository_->Update(std::move(company));

        return employeeId;
    }

    template<EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
    void UnitOfWork<TEmployeeRepository, TCompanyRepository>::RemoveEmployee(int companyId, int employeeId) {
        // remove employee from the company
        auto company = CompanyRepository_->Get(companyId);
        std::erase_if(company.Employees.begin(), company.Employees.end(), [employeeId](const auto &employee) {
            return employee.Id == employeeId;
        });

        // remove employee
        EmployeeRepository_->Remove(employeeId);
    }

    template<EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
    void UnitOfWork<TEmployeeRepository, TCompanyRepository>::RotateEmployee(int fromCompanyId, int toCompanyId,
                                                                             int employeeId) {
        if (fromCompanyId == toCompanyId) {
            return;
        }

        // remove employee from the company
        auto fromCompany = CompanyRepository_->Get(fromCompanyId);
        std::erase_if(fromCompany.Employees.begin(), fromCompany.Employees.end(), [employeeId](const auto &employee) {
            return employee.Id == employeeId;
        });

        // add employee to another company
        auto toCompany = CompanyRepository_->Get(toCompanyId);
        toCompany.push_back(employeeId);
        CompanyRepository_->Update(std::move(toCompany));

        // update companyId of employee
        auto employee = EmployeeRepository_->Get(employeeId);
        employee.CompanyId = toCompanyId;
        EmployeeRepository_->Update(std::move(employee));
    }

    template<EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
    Enumerable<Employee> UnitOfWork<TEmployeeRepository, TCompanyRepository>::GetCompanyEmployees(int companyId) {
        auto company = CompanyRepository_->Get(companyId);
        for (auto employeeId : company.Employees) {
            co_yield EmployeeRepository_->Get(employeeId);
        }
    }

    template<EmployeeRepository TEmployeeRepository, CompanyRepository TCompanyRepository>
    UnitOfWork<TEmployeeRepository, TCompanyRepository>::UnitOfWork(TEmployeeRepository* employeeRepository,
                                                                    TCompanyRepository* companyRepository)
                                                                    : EmployeeRepository_(employeeRepository)
                                                                    , CompanyRepository_(companyRepository) {
    }
}
