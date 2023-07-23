#pragma once

#include "../domain/department.h"

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

}
