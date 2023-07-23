#include "unit_of_work.h"

#include <algorithm>

namespace Example::Dao {

    using namespace Example::Domain;
    using namespace Example::Util;

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    int UnitOfWork<TEmployeeRepository, TDepartmentRepository>::AddDepartment(const std::string& departmentName) {
        return DepartmentRepository_->Add({
            .Name = departmentName,
        });
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    int
    UnitOfWork<TEmployeeRepository, TDepartmentRepository>::AddEmployee(int departmentId, const std::string& employeeName) {
        // create employee
        auto employeeId = EmployeeRepository_->Add({
            .DepartmentId = departmentId,
            .Name = employeeName,
       });

        // add employee to department
        auto department = DepartmentRepository_->Get(departmentId);
        department.Employees.push_back(employeeId);
        DepartmentRepository_->Update(std::move(department));

        return employeeId;
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    void UnitOfWork<TEmployeeRepository, TDepartmentRepository>::RemoveDepartment(int departmentId) {
        // remove employees
        auto department = DepartmentRepository_->Get(departmentId);
        for (auto employeeId : department.Employees) {
            EmployeeRepository_->Remove(employeeId);
        }

        DepartmentRepository_->Remove(departmentId);
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    void UnitOfWork<TEmployeeRepository, TDepartmentRepository>::RemoveEmployee(int departmentId, int employeeId) {
        // remove employee from the department
        auto department = DepartmentRepository_->Get(departmentId);
        std::erase_if(department.Employees.begin(), department.Employees.end(), [employeeId](const auto& employee) {
            return employee.Id == employeeId;
        });

        // remove employee
        EmployeeRepository_->Remove(employeeId);
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    void UnitOfWork<TEmployeeRepository, TDepartmentRepository>::RotateEmployee(int fromDepartmentId, int toDepartmentId,
                                                                                int employeeId) {
        if (fromDepartmentId == toDepartmentId) {
            return;
        }

        // remove employee from the department
        auto fromDepartment = DepartmentRepository_->Get(fromDepartmentId);
        std::erase_if(fromDepartment.Employees.begin(), fromDepartment.Employees.end(), [employeeId](const auto& employee) {
            return employee.Id == employeeId;
        });

        // add employee to another department
        auto toDepartment = DepartmentRepository_->Get(toDepartmentId);
        toDepartment.push_back(employeeId);
        DepartmentRepository_->Update(std::move(toDepartment));

        // update departmentId of employee
        auto employee = EmployeeRepository_->Get(employeeId);
        employee.DepartmentId = toDepartmentId;
        EmployeeRepository_->Update(std::move(employee));
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    Enumerable<Department> UnitOfWork<TEmployeeRepository, TDepartmentRepository>::GetDepartments() {
        for (auto department : DepartmentRepository_->GetAll()) {
            co_yield department;
        }
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    Enumerable<Employee> UnitOfWork<TEmployeeRepository, TDepartmentRepository>::GetDepartmentEmployees(int departmentId) {
        auto department = DepartmentRepository_->Get(departmentId);
        for (auto employeeId : department.Employees) {
            co_yield EmployeeRepository_->Get(employeeId);
        }
    }

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    UnitOfWork<TEmployeeRepository, TDepartmentRepository>::UnitOfWork(TEmployeeRepository* employeeRepository,
                                                                    TDepartmentRepository* departmentRepository)
                                                                    : EmployeeRepository_(employeeRepository)
                                                                    , DepartmentRepository_(departmentRepository) {
    }
}
