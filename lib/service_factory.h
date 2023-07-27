#pragma once

namespace IOC {

    template <typename TService>
    struct ServiceFactory;

    template <typename TService>
    struct ServiceFactory {
        static constexpr TService Create(const auto&) {
            return TService();
        }
    };

}
