#pragma once

#include "../../lib/ioc.h"
#include "exposure.h"
#include "repository.h"
#include "unit_of_work.h"

template<>
struct IOC::Binding<Example::Dao::EmployeeRepositoryDescriptor> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Employee>;
};

template<>
struct IOC::Binding<Example::Dao::DepartmentRepositoryDescriptor> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Department>;
};

template<typename ...TRepositories>
struct IOC::ServiceFactory<Example::Dao::UnitOfWork<TRepositories...>> {
    static Example::Dao::UnitOfWork<TRepositories...> Create(const auto& container) {
        return {
            container.template Resolve<Example::Dao::EmployeeRepositoryDescriptor>(),
            container.template Resolve<Example::Dao::DepartmentRepositoryDescriptor>()
        };
    }
};
