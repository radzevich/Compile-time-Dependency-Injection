#pragma once

#include "../lib/ioc.h"
#include "dao/bootstrap.h"
#include "exposure.h"

template<>
struct IOC::Binding<Example::Services::UnitOfWorkDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Dao::UnitOfWork<
            IOC::Binding<Example::Dao::EmployeeRepositoryDescriptor>::TService,
            IOC::Binding<Example::Dao::DepartmentRepositoryDescriptor>::TService>;
};


