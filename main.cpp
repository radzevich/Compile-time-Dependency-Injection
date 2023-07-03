#include <iostream>
#include <optional>

struct ADescriptor {};
struct BDescriptor {};
struct CDescriptor {};

class A {
public:
    A() {
        std::cout << "constr: A" << '\n';
    }

    void Whoami() {
        std::cout << "A" << '\n';
    }

    ~A() {
        std::cout << "destr: A" << '\n';
    }
};

class B {
public:
    B() {
        std::cout << "constr: B" << '\n';
    }

    void Whoami() {
        std::cout << "B" << '\n';
    }

    ~B() {
        std::cout << "destr: B" << '\n';
    }
};

class C {
private:
    A* a_;
    B* b_;
public:
    C(A* a, B* b) : a_(a), b_(b) {
        std::cout << "constr: C" << '\n';
    }

    void Whoami() {
        std::cout << "C" << '\n';
    }

    ~C() {
        std::cout << "destr: C" << '\n';
    }
};

template <typename TDescriptor>
struct Binding;

template <typename TDescriptor>
struct Singleton {
    using TType = TDescriptor;
};

template <typename TDescriptor>
struct Scoped {
    using TType = TDescriptor;
};

template <typename TContainer, typename TDescriptor>
struct ServiceResolver {
    using TService = typename Binding<TDescriptor>::TService;

    explicit ServiceResolver(const TContainer& container) : Container_(container) {
    }

    TService Resolve() {
        return TContainer::template Create<TService>(Container_);
    }

    template <typename TFactory>
    TService Resolve() {
        return TContainer::template Create<TService, TFactory>(Container_);
    }

private:
    const TContainer& Container_;
};

template <typename TContainer, typename TDescriptor>
struct ServiceResolver<TContainer, Scoped<TDescriptor>> {
    using TService = typename Binding<Scoped<TDescriptor>>::TService;

    explicit ServiceResolver(const TContainer& container) : Container_(container) {
    }

    TService* Resolve() {
        return TContainer::template GetOrCreate<TService>(Container_);
    }

    template <typename TFactory>
    TService* Resolve() {
        return TContainer::template GetOrCreate<TService, TFactory>(Container_);
    }

private:
    const TContainer& Container_;
};

template <typename TContainer, typename TDescriptor>
struct ServiceResolver<TContainer, Singleton<TDescriptor>> {
    using TService = typename Binding<Singleton<TDescriptor>>::TService;

    explicit ServiceResolver(const TContainer&) {
    }

    TService* Resolve() {
        return TContainer::template GetOrCreate<TService>(GetContainerSingleton());
    }

    template <typename TFactory>
    TService* Resolve() {
        return TContainer::template GetOrCreate<TService>(GetContainerSingleton());
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

    template<typename TDescriptor>
    [[nodiscard]] auto Resolve() const -> decltype(auto) {
        using TBinding = Binding<TDescriptor>;
        using TService = typename TBinding::TService;

        ServiceResolver<TContainer, TDescriptor> serviceResolver(*this);

        if constexpr (std::is_invocable_r_v<TService, TBinding, TContainer>) {
            return serviceResolver.template Resolve<TBinding>();
        } else {
            return serviceResolver.Resolve();
        }
    }

    TContainer BeginScope() {
        return Container<TDescriptors...>();
    }

    template<typename TService, typename TFactory>
    [[nodiscard]] static TService* GetOrCreate(const TContainer& container) {
        static_assert(std::is_invocable_r_v<TService, TFactory, TContainer>);

        auto& optInstance = std::get<std::optional<TService>>(container.Instances_);
        if (optInstance.has_value()) {
            return std::addressof(optInstance.value());
        } else {
            TFactory factory;
            return std::addressof(optInstance.emplace(factory(container)));
        }
    }

    template<typename TService>
    [[nodiscard]] static TService* GetOrCreate(const TContainer& container) {
        auto& optInstance = std::get<std::optional<TService>>(container.Instances_);
        if (optInstance.has_value()) {
            return std::addressof(optInstance.value());
        } else {
            return std::addressof(optInstance.emplace());
        }
    }

    template<typename TService, typename TFactory>
    [[nodiscard]] static TService Create(const TContainer& container) {
        static_assert(std::is_invocable_r_v<TService, TFactory, TContainer>);

        TFactory factory;
        return factory(container);
    }

    template<typename TService>
    [[nodiscard]] static TService Create() {
        return TService();
    }
};

template <>
struct Binding<Singleton<ADescriptor>> {
    using TService = A;
};

template <>
struct Binding<Scoped<BDescriptor>> {
    using TService = B;
};

using TServiceContainer = Container<
        Singleton<ADescriptor>,
        Scoped<BDescriptor>,
        CDescriptor>;

template <>
struct Binding<CDescriptor> {
    using TService = C;

    auto operator()(const TServiceContainer& container) {
        auto a = container.Resolve<Singleton<ADescriptor>>();
        auto b = container.Resolve<Scoped<BDescriptor>>();

        return C(a, b);
    }
};


int main() {
    TServiceContainer ioc;

    auto a = ioc.Resolve<Singleton<ADescriptor>>();
    a->Whoami();

    auto b = ioc.Resolve<Scoped<BDescriptor>>();
    b->Whoami();

    auto c = ioc.Resolve<CDescriptor>();
    c.Whoami();

    return 0;
}
