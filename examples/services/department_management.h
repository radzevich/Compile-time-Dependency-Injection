#pragma once

#include "../domain/department.h"
#include <concepts>
#include <string>

namespace Example::Services {

    template<typename TUnitOfWork, typename TLogger>
    class DepartmentManagementService {
    private:
        TUnitOfWork* UnitOfWork_;
        TLogger* Logger_;

    public:
        DepartmentManagementService(TUnitOfWork* unitOfWork, TLogger* logger);

        int Create(const std::string& departmentName);
        void Remove(int departmentId);
    };

    template<typename TUnitOfWork, typename TLogger>
    void DepartmentManagementService<TUnitOfWork, TLogger>::Remove(int departmentId) {
        Logger_->Info("Removing department " + std::to_string(departmentId));
        UnitOfWork_->RemoveDepartment(departmentId);
        Logger_->Info("Department " + std::to_string(departmentId) + " successfully removed");
    }

    template<typename TUnitOfWork, typename TLogger>
    int DepartmentManagementService<TUnitOfWork, TLogger>::Create(const std::string& departmentName) {
        Logger_->Info("Creating department '" + departmentName + "'");
        auto departmentId = UnitOfWork_->AddDepartment(departmentName);
        Logger_->Info("Department '" + departmentName + "' successfully created with id: " + std::to_string(departmentId));

        return departmentId;
    }

    template<typename TUnitOfWork, typename TLogger>
    DepartmentManagementService<TUnitOfWork, TLogger>::DepartmentManagementService(TUnitOfWork* unitOfWork,
                                                                                   TLogger* logger)
        : UnitOfWork_(unitOfWork)
        , Logger_(logger) {
        Logger_->Info("Created DepartmentManagementService");
    }
}
