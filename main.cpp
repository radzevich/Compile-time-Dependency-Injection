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

template <typename ... TServices>
class ServiceScope {
private:
    std::tuple<std::optional<TServices>...> Instances_;

public:
    template<typename TContainer, typename TService, typename TFactory>
    [[nodiscard]] TService* Resolve(const TContainer& caller) {
        static_assert(std::is_invocable_r_v<TService, TFactory, TContainer&>);

        auto& optInstance = std::get<std::optional<TService>>(Instances_);
        if (optInstance.has_value()) {
            return std::addressof(optInstance.value());
        } else {
            TFactory factory;
            return std::addressof(optInstance.emplace(factory(caller)));
        }
    }

    template<typename TService>
    [[nodiscard]] TService* Resolve() {
        auto& optInstance = std::get<std::optional<TService>>(Instances_);
        if (optInstance.has_value()) {
            return std::addressof(optInstance.value());
        } else {
            return std::addressof(optInstance.emplace());
        }
    }
};

template <typename ... TDescriptors>
class Container {
private:
    using TThis = Container<TDescriptors...>;
    using TScope = ServiceScope<typename Binding<TDescriptors>::TService...>;
    mutable TScope Scope_;

public:
    template<typename TDescriptor>
    [[nodiscard]] auto Resolve() const -> decltype(auto) {
        using TBinding = Binding<TDescriptor>;
        using TService = typename TBinding::TService;

        if constexpr (std::is_invocable_r_v<TService, TBinding, TThis>) {
            return Scope_.template Resolve<TThis, TService, TBinding>(*this);
        } else {
            return Scope_.template Resolve<TService>();
        }
    }
};

template <>
struct Binding<ADescriptor> {
    using TService = A;
};

template <>
struct Binding<BDescriptor> {
    using TService = B;
};

using TServiceContainer = Container<ADescriptor, BDescriptor, CDescriptor>;

template <>
struct Binding<CDescriptor> {
    using TService = C;

    auto operator()(const TServiceContainer& container) {
        auto a = container.Resolve<ADescriptor>();
        auto b = container.Resolve<BDescriptor>();

        return C(a, b);
    }
};


int main() {
    TServiceContainer ioc;

    auto a = ioc.Resolve<ADescriptor>();
    a->Whoami();

    auto b = ioc.Resolve<BDescriptor>();
    b->Whoami();

    auto c = ioc.Resolve<CDescriptor>();
    c->Whoami();

    return 0;
}
