#pragma once

#include "../domain/department.h"
#include "../domain/employee.h"
#include "exposure.h"
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

template<typename ...TDeps>
struct IOC::ServiceFactory<Example::Services::EmployeeManagementService<TDeps...>> {
    static Example::Services::EmployeeManagementService<TDeps...> Create(const auto& container) {
        return {
            container.template Resolve<Example::Services::UnitOfWorkDescriptor>(),
            container.template Resolve<Example::Services::LoggerDescriptor>()
        };
    }
};
