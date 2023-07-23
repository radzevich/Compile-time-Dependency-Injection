#pragma once

#include <string>
#include <vector>

namespace Example::Domain {

    struct Department {
        int Id;
        std::string Name;
        std::vector<int> Employees;
    };

}