#pragma once

#include "../../lib/ioc_2.h"
#include "unit_of_work.h"
#include "repository.h"

struct EmployeeRepositoryDescriptor {};

struct CompanyRepositoryDescriptor {};

struct UnitOfWorkDescriptor {};

template <typename ...TRepositories>
struct IOC2::ServiceFactory<UnitOfWork<TRepositories...>> {
    template <typename TContainer>
    static UnitOfWork<TRepositories...> Create(const TContainer& container) {
        auto* employeeRepository = container.template Resolve<EmployeeRepositoryDescriptor>();
        auto* companyRepository = container.template Resolve<CompanyRepositoryDescriptor>();

        return UnitOfWork(employeeRepository, companyRepository);
    }
};