#pragma once

#include "binding.h"
#include "lifetime_manager.h"

namespace IOC {

    template <typename TDescriptor, typename... TDescriptors>
    concept OneOf = (std::same_as<TDescriptor, TDescriptors> || ...);

    template <typename ...TDescriptors>
    class Container;

    template <typename ...TDescriptors>
    class Container {
    private:
        std::tuple<LifetimeManager<TDescriptors>...> LifetimeManagers_;

    public:
        template <OneOf<TDescriptors...> TDescriptor>
        constexpr auto Resolve() {
            return std::get<LifetimeManager<TDescriptor>>(LifetimeManagers_)
                .GetOrCreate(*this);
        }

        template <typename TDescriptor>
        constexpr auto Resolve() {
            return std::nullopt;
        }
    };

    template <typename ...TContainerArs, typename ...Rest>
    class Container<Container<TContainerArs...>, Rest...>
        : public Container<Rest..., TContainerArs...> {
    };

}
