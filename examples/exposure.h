#pragma once

#include "../lib/ioc.h"
#include "dao/exposure.h"
#include "services/exposure.h"

namespace Example {

    using TContainer = class IOC::ServiceCollection<
            Services::TContainer,
            Dao::TContainer>;

}
