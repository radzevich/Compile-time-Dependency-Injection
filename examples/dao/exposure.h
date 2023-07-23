#pragma once

#include "../../lib/ioc.h"

namespace Example::Dao {

    struct EmployeeRepositoryDescriptor {};

    struct DepartmentRepositoryDescriptor {};

    using TContainer = class IOC::ServiceCollection<
            EmployeeRepositoryDescriptor,
            DepartmentRepositoryDescriptor>;

}
