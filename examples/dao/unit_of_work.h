#pragma once

#include "../domain//department.h"
#include "../domain/employee.h"
#include "../utils/enumerable.h"
#include "../utils/input_range.h"
#include "exposure.h"
#include <concepts>
#include <string>

namespace Example::Dao {

    template<class TEmployeeRepository>
    concept EmployeeRepository = requires(TEmployeeRepository& repository, Domain::Employee employee) {
        { repository.Get(employee.Id) } -> std::same_as<Domain::Employee>;
        { repository.Add(employee) } -> std::same_as<decltype(employee.Id)>;
        { repository.Update(employee) };
        { repository.Remove(employee.Id) };
        requires Util::InputRange<decltype(repository.GetAll()), Domain::Employee>;
    };

    template<class TDepartmentRepository>
    concept DepartmentRepository = requires(TDepartmentRepository& repository, Domain::Department department) {
        { repository.Get(department.Id) } -> std::same_as<Domain::Department>;
        { repository.Add(department) } -> std::same_as<decltype(department.Id)>;
        { repository.Update(department) };
        { repository.Remove(department.Id) };
        requires Util::InputRange<decltype(repository.GetAll()), Domain::Department>;
    };

    template<EmployeeRepository TEmployeeRepository, DepartmentRepository TDepartmentRepository>
    class UnitOfWork {
    private:
        TEmployeeRepository* EmployeeRepository_;
        TDepartmentRepository* DepartmentRepository_;

    public:
        UnitOfWork(TEmployeeRepository* employeeRepository, TDepartmentRepository* departmentRepository);

        int AddDepartment(const std::string& departmentName);
        int AddEmployee(int departmentId, const std::string& employeeName);
        void RemoveDepartment(int departmentId);
        void RemoveEmployee(int departmentId, int employeeId);
        Example::Util::Enumerable<Domain::Department> GetDepartments();
        Example::Util::Enumerable<Domain::Employee> GetDepartmentEmployees(int departmentId);
        void RotateEmployee(int fromDepartmentId, int toDepartmentId, int employeeId);
    };

}

#include "unit_of_work.cpp"