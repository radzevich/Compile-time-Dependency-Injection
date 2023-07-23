#pragma once

#include "../../lib/ioc.h"

namespace Example::Services {

    struct UnitOfWorkDescriptor {};

    struct LoggerDescriptor {};

    struct PrinterDescriptor {};

    using TContainer = class IOC::ServiceCollection<
            UnitOfWorkDescriptor>;

}