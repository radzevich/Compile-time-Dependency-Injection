#pragma once

#include "../domain/department.h"
#include "../domain/employee.h"
#include "../exposure.h"
#include "../infra/abstract/logger_descriptor.h"
#include <concepts>
#include <string>

namespace Example::Services {

    template<
            typename TUnitOfWork = Dao::Abstract::UnitOfWorkDescriptor,
            typename TLogger = Infra::Abstract::LoggerDescriptor>
    class EmployeeManagementService {
    private:
        TUnitOfWork* UnitOfWork_;
        TLogger* Logger_;

    public:
        EmployeeManagementService(TUnitOfWork* unitOfWork, TLogger* logger);

        int Hire(int departmentId, const std::string& employeeName);
        void Fire(int departmentId, int employeeId);
        void Rotate(int employeeId, int fromDepartment, int toDepartment);
    };

    template<typename TUnitOfWork, typename TLogger>
    EmployeeManagementService<TUnitOfWork, TLogger>::EmployeeManagementService(TUnitOfWork* unitOfWork, TLogger* logger)
        : UnitOfWork_(unitOfWork)
        , Logger_(logger) {
        Logger_->Info("Created EmployeeManagementService");
    }

    template<typename TUnitOfWork, typename TLogger>
    void EmployeeManagementService<TUnitOfWork, TLogger>::Rotate(int employeeId, int fromDepartment, int toDepartment) {
        Logger_->Info("Rotate employee = " + std::to_string(employeeId)
                    + " from department = " + std::to_string(fromDepartment)
                    + " to department = " + std::to_string(toDepartment));

        UnitOfWork_->RotateEmployee(fromDepartment, toDepartment, employeeId);
    }

    template<typename TUnitOfWork, typename TLogger>
    void EmployeeManagementService<TUnitOfWork, TLogger>::Fire(int departmentId, int employeeId) {
        Logger_->Info("Fire employee = " + std::to_string(employeeId)
                    + " from department = " + std::to_string(departmentId));

        UnitOfWork_->RemoveEmployee(departmentId, employeeId);
    }

    template<typename TUnitOfWork, typename TLogger>
    int EmployeeManagementService<TUnitOfWork, TLogger>::Hire(int departmentId, const std::string& employeeName) {
        auto employeeId = UnitOfWork_->AddEmployee(departmentId, employeeName);

        Logger_->Info("Hire employee = " + employeeName
                    + " with id = " + std::to_string(employeeId)
                    + " to department = " + std::to_string(departmentId));

        return employeeId;
    }
}
