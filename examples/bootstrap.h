#pragma once

#include "../lib/ioc.h"
#include "exposure.h"
#include "domain/employee.h"
#include "domain/department.h"
#include "dao/repository.h"
#include "dao/unit_of_work.h"

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

template<>
struct IOC::Binding<Example::Services::UnitOfWorkDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Dao::UnitOfWork<
            IOC::Binding<Example::Dao::EmployeeRepositoryDescriptor>::TService,
            IOC::Binding<Example::Dao::DepartmentRepositoryDescriptor>::TService>;
};


