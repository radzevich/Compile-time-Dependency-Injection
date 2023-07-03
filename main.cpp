#include <iostream>

struct ADescriptor {};
struct BDescriptor {};
struct CDescriptor {};

class A {
public:
    void Whoami() {
        std::cout << "A" << std::endl;
    }
};

class B {
public:
    void Whoami() {
        std::cout << "B" << std::endl;
    }
};

class C {
private:
    A a_;
    B b_;
public:
    C(A a, B b) : a_(a), b_(b) {
    }

    void Whoami() {
        std::cout << "C" << std::endl;
    }
};

template <typename TDescriptor>
struct Binding;

class Container {
public:
    template<typename TDescriptor>
    [[nodiscard]] auto Resolve() const -> decltype(auto) {
        using TBinding = Binding<TDescriptor>;
        using TService = typename TBinding::TService;

        if constexpr (std::is_invocable_r_v<TService, TBinding, Container>) {
            TBinding binding;
            return binding(*this);
        } else {
            TService service;
            return service;
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

template <>
struct Binding<CDescriptor> {
    using TService = C;

    auto operator()(const Container& container) {
        auto a = container.Resolve<ADescriptor>();
        auto b = container.Resolve<BDescriptor>();

        return TService(a, b);
    }
};


int main() {
    Container ioc;

    ioc.Resolve<ADescriptor>().Whoami();
    ioc.Resolve<BDescriptor>().Whoami();
    ioc.Resolve<CDescriptor>().Whoami();

    return 0;
}
