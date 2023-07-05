#pragma once

#include <optional>

namespace IOC {

    template <typename TDescriptor>
    struct Binding;

    template <typename TDescriptor>
    struct Singleton {};

    template <typename TDescriptor>
    struct Scoped {};

    template <typename TContainer, typename TDescriptor>
    struct ServiceResolver {
        using TService = typename Binding<TDescriptor>::TService;

        explicit ServiceResolver(const TContainer& container) : Container_(container) {
        }

        TService Resolve() {
            return Container_.template Create<TService>();
        }

        template <typename TFactory>
        TService Resolve() {
            return Container_.template Create<TService, TFactory>();
        }

    private:
        const TContainer& Container_;
    };

    template <typename TContainer, typename TDescriptor>
    struct ServiceResolver<TContainer, Scoped<TDescriptor>> {
        using TService = typename Binding<TDescriptor>::TService;

        explicit ServiceResolver(const TContainer& container) : Container_(container) {
        }

        TService* Resolve() {
            return Container_.template GetOrCreate<TService>();
        }

        template <typename TFactory>
        TService* Resolve() {
            return Container_.template GetOrCreate<TService, TFactory>();
        }

    private:
        const TContainer& Container_;
    };

    template <typename TContainer, typename TDescriptor>
    struct ServiceResolver<TContainer, Singleton<TDescriptor>> {
        using TService = typename Binding<TDescriptor>::TService;

        explicit ServiceResolver(const TContainer&) {
        }

        TService* Resolve() {
            return GetContainerSingleton().template GetOrCreate<TService>();
        }

        template <typename TFactory>
        TService* Resolve() {
            return GetContainerSingleton().template GetOrCreate<TService>();
        }

    private:
        static const TContainer& GetContainerSingleton() {
            static TContainer instance;
            return instance;
        }
    };

    template <typename ... TDescriptors>
    class Container {
    private:
        using TContainer = Container<TDescriptors...>;

    private:
        mutable std::tuple<std::optional<typename Binding<TDescriptors>::TService>...> Instances_;

    public:
        Container() = default;
        ~Container() = default;

        Container(const Container& rhs) = delete;
        Container& operator=(const Container& rhs) = delete;

        Container(Container&& rhs) noexcept = default;
        Container& operator=(Container&& rhs) noexcept = default;

        static TContainer BeginScope() {
            return Container<TDescriptors...>();
        }

        template<typename TDescriptor, typename TBinding = Binding<TDescriptor>>
        [[nodiscard]] auto Resolve() const -> decltype(auto) {
            ServiceResolver<TContainer, TDescriptor> serviceResolver(*this);

            if constexpr (std::is_invocable_v<TBinding, TContainer>) {
                return serviceResolver.template Resolve<TBinding>();
            } else {
                return serviceResolver.Resolve();
            }
        }

        template<typename TService, typename TFactory>
        [[nodiscard]] TService* GetOrCreate() const {
            static_assert(std::is_invocable_r_v<TService, TFactory, TContainer>);

            auto& optInstance = std::get<std::optional<TService>>(Instances_);
            if (optInstance.has_value()) {
                return std::addressof(optInstance.value());
            } else {
                TFactory factory;
                return std::addressof(optInstance.emplace(factory(*this)));
            }
        }

        template<typename TService>
        [[nodiscard]] TService* GetOrCreate() const {
            auto& optInstance = std::get<std::optional<TService>>(Instances_);
            if (optInstance.has_value()) {
                return std::addressof(optInstance.value());
            } else {
                return std::addressof(optInstance.emplace());
            }
        }

        template<typename TService, typename TFactory>
        [[nodiscard]] TService Create() const {
            static_assert(std::is_invocable_r_v<TService, TFactory, TContainer>);

            TFactory factory;
            return factory(*this);
        }

        template<typename TService>
        [[nodiscard]] TService Create() const {
            return TService();
        }
    };

}
