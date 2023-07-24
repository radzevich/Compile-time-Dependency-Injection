#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace Example::Infra {

    class Logger {
    private:
        std::string Format_;

    public:
        explicit Logger(std::string format);
        void Info(std::string_view message);
    };

}