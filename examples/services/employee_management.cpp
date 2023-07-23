#include "employee_management.h"

namespace Example::Services {

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    EmployeeManagementService<TUnitOfWork, TLogger>::EmployeeManagementService(TUnitOfWork* unitOfWork, TLogger* logger)
        : UnitOfWork_(unitOfWork)
        , Logger_(logger) {
    }

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    void EmployeeManagementService<TUnitOfWork, TLogger>::Rotate(int employeeId, int fromDepartment, int toDepartment) {
        Logger_->Info("Rotate employee = " + std::to_string(employeeId)
                    + " from department = " + std::to_string(fromDepartment)
                    + " to department = " + std::to_string(toDepartment));

        UnitOfWork_->RotateEmployee(fromDepartment, toDepartment, employeeId);
    }

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    void EmployeeManagementService<TUnitOfWork, TLogger>::Fire(int departmentId, int employeeId) {
        Logger_->Info("Fire employee = " + std::to_string(employeeId)
                      + " from department = " + std::to_string(departmentId));

        UnitOfWork_->RemoveEmployee(departmentId, employeeId);
    }

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    void EmployeeManagementService<TUnitOfWork, TLogger>::Hire(int departmentId, const std::string& employeeName) {
        Logger_->Info("Hire employee = " + employeeName
                      + " to department = " + std::to_string(departmentId));

        UnitOfWork_->AddEmployee(departmentId, employeeName);
    }

}