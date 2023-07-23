#include "department_management.h"

namespace Example::Services {

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