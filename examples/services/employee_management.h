#pragma once

#include "../domain/department.h"
#include "../domain/employee.h"

#include <concepts>
#include <string>

namespace Example::Services {

    template<class TUnitOfWork>
    concept UnitOfWork = requires(TUnitOfWork& uow, const std::string& employeeName, int employeeId, int departmentId) {
        { uow.AddEmployee(departmentId, employeeName) } -> std::same_as<int>;
        { uow.RotateEmployee(departmentId, departmentId, employeeId) };
        { uow.RemoveEmployee(departmentId, employeeId) };
    };

    template<class TLogger>
    concept Logger = requires(TLogger& logger, const char* message) {
        { logger.Info(message) };
    };

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    class EmployeeManagementService {
    private:
        TUnitOfWork* UnitOfWork_;
        TLogger* Logger_;

    public:
        EmployeeManagementService(TUnitOfWork* unitOfWork, TLogger* logger);

        void Hire(int departmentId, const std::string& employeeName);
        void Fire(int departmentId, int employeeId);
        void Rotate(int employeeId, int fromDepartment, int toDepartment);
    };

}
