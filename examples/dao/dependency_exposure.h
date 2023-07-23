#pragma once

#include "../../lib/ioc_2.h"
#include "unit_of_work.h"
#include "repository.h"

namespace Example::Dao {

    struct EmployeeRepositoryDescriptor {
    };

    struct CompanyRepositoryDescriptor {
    };

    struct UnitOfWorkDescriptor {
    };
}

template<typename ...TRepositories>
struct IOC2::ServiceFactory<Example::Dao::UnitOfWork<TRepositories...>> {
    template<typename TContainer>
    static Example::Dao::UnitOfWork<TRepositories...> Create(const TContainer &container) {
        auto *employeeRepository = container.template Resolve<Example::Dao::EmployeeRepositoryDescriptor>();
        auto *companyRepository = container.template Resolve<Example::Dao::CompanyRepositoryDescriptor>();

        return Example::Dao::UnitOfWork(employeeRepository, companyRepository);
    }
};
