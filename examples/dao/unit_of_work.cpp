#include "unit_of_work.h"

#include <algorithm>

namespace Example::Dao {

    using namespace Example::Domain;
    using namespace Example::Util;

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    int UnitOfWork<TEmployeeRepository, TDepartmentRepository>::AddDepartment(const std::string& departmentName) {
        return DepartmentRepository_->Add({
            .Name = departmentName,
        });
    }

    template<typename TEmployeeRepository, typename TDepartmentRepository>
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

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    void UnitOfWork<TEmployeeRepository, TDepartmentRepository>::RemoveDepartment(int departmentId) {
        // remove employees
        auto department = DepartmentRepository_->Get(departmentId);
        for (auto employeeId : department.Employees) {
            EmployeeRepository_->Remove(employeeId);
        }

        DepartmentRepository_->Remove(departmentId);
    }

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    void UnitOfWork<TEmployeeRepository, TDepartmentRepository>::RemoveEmployee(int departmentId, int employeeId) {
        // remove employee from the department
        auto department = DepartmentRepository_->Get(departmentId);
        department.Employees.erase(
            std::remove(department.Employees.begin(), department.Employees.end(), employeeId),
            department.Employees.end());

        // remove employee
        EmployeeRepository_->Remove(employeeId);
    }

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    void UnitOfWork<TEmployeeRepository, TDepartmentRepository>::RotateEmployee(int fromDepartmentId, int toDepartmentId,
                                                                                int employeeId) {
        if (fromDepartmentId == toDepartmentId) {
            return;
        }

        // remove employee from the department
        Department fromDepartment = DepartmentRepository_->Get(fromDepartmentId);
        fromDepartment.Employees.erase(
            std::remove(fromDepartment.Employees.begin(), fromDepartment.Employees.end(), employeeId),
            fromDepartment.Employees.end());
        DepartmentRepository_->Update(fromDepartment);

        // add employee to another department
        auto toDepartment = DepartmentRepository_->Get(toDepartmentId);
        toDepartment.Employees.push_back(employeeId);
        DepartmentRepository_->Update(std::move(toDepartment));

        // update departmentId of employee
        auto employee = EmployeeRepository_->Get(employeeId);
        employee.DepartmentId = toDepartmentId;
        EmployeeRepository_->Update(std::move(employee));
    }

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    Enumerable<Department> UnitOfWork<TEmployeeRepository, TDepartmentRepository>::GetDepartments() {
        for (auto department : DepartmentRepository_->GetAll()) {
            co_yield department;
        }
    }

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    Enumerable<Employee> UnitOfWork<TEmployeeRepository, TDepartmentRepository>::GetDepartmentEmployees(int departmentId) {
        auto department = DepartmentRepository_->Get(departmentId);
        for (auto employeeId : department.Employees) {
            co_yield EmployeeRepository_->Get(employeeId);
        }
    }

    template<typename TEmployeeRepository, typename TDepartmentRepository>
    UnitOfWork<TEmployeeRepository, TDepartmentRepository>::UnitOfWork(TEmployeeRepository* employeeRepository,
                                                                    TDepartmentRepository* departmentRepository)
                                                                    : EmployeeRepository_(employeeRepository)
                                                                    , DepartmentRepository_(departmentRepository) {
    }
}
