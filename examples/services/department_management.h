#pragma once

#include "../domain/department.h"
#include "exposure.h"
#include <concepts>
#include <string>

namespace Example::Services {

    template<class TUnitOfWork>
    concept UnitOfWork = requires(TUnitOfWork& uow, const std::string& departmentName, int departmentId) {
        { uow.AddDepartment(departmentId, departmentName) } -> std::same_as<int>;
        { uow.RemoveDepartment(departmentId) };
    };

    template<class TLogger>
    concept Logger = requires(TLogger& logger, const char* message) {
        { logger.Info(message) };
    };

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    class DepartmentManagementService {
    private:
        TUnitOfWork* UnitOfWork_;
        TLogger* Logger_;

    public:
        DepartmentManagementService(TUnitOfWork* unitOfWork, TLogger* logger);

        int Create(const std::string& departmentName);
        void Remove(int departmentId);
    };

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    void DepartmentManagementService<TUnitOfWork, TLogger>::Remove(int departmentId) {
        Logger_->Info("Removing department " + std::to_string(departmentId));
        UnitOfWork_->RemoveDepartment(departmentId);
        Logger_->Info("Department " + std::to_string(departmentId) + " successfully removed");
    }

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    int DepartmentManagementService<TUnitOfWork, TLogger>::Create(const std::string& departmentName) {
        Logger_->Info("Creating department " + departmentName);
        auto departmentId = UnitOfWork_->AddDepartment(departmentName);
        Logger_->Info("Department " + departmentName + " successfully created with id: " + std::to_string(departmentId));
    }

    template<UnitOfWork TUnitOfWork, Logger TLogger>
    DepartmentManagementService<TUnitOfWork, TLogger>::DepartmentManagementService(TUnitOfWork* unitOfWork,
                                                                                   TLogger* logger)
        : UnitOfWork_(unitOfWork)
        , Logger_(logger) {
    }
}

template<typename ...TDeps>
struct IOC::ServiceFactory<Example::Services::DepartmentManagementService<TDeps...>> {
    static Example::Services::DepartmentManagementService<TDeps...> Create(const auto& container) {
        return {
            container.template Resolve<Example::Services::UnitOfWorkDescriptor>(),
            container.template Resolve<Example::Services::LoggerDescriptor>()
        };
    }
};