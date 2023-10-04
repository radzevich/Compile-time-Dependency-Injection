# IOC
The presented library is used for dependency injection at compile time. For this reason, it utilizes template arguments rather than interfaces, as libraries operating at runtime typically do.

# How to use?
The "FordMustang" class depends on a specific type of engine and transmission. This means that replacing one of these two components will require changes to the code of the entire class "FordMustang"
```cpp
class FordMustang {
private:
    V4EcoBoost Engine_;
    Automatic10Speed Transmission_;
...
};
```
Here are the steps to make tuning the Mustang easier.

## Step 1. Invert dependencies
The engine and transmission are inserted into the car; they are not created inside it. We will do the same with dependencies: we will accept dependencies or references to them (we'll decide which one later) through the constructor, rather than creating them inside the class itself. The engine and transmission types are hidden behind template arguments.
```cpp
template <typename TEngine, typename TTransmission>
class FordMustang {
private:
    TEngine Engine_;
    TTransmission Transmission_;

public:
    FordMustang(TEngine engine, TTransmission transmission)
        : Engine_(std::move(engine))
        , Transmission_(std::move(transmission)) {
    }
...
};
```

## Step 2. Hide implementations behind descriptors
A descriptor is a special type that hides the implementation. In the case of dynamic polymorphism, an interface would serve as a substitute for such a type. However, since we are using templates, interfaces are absent.
```cpp
struct EngineDescriptor {};

struct TransmissionDescriptor {};
```
Now the descriptor needs to be specified as the default type of the template arguments. When the dependencies of the "FordMustang" class are resolved, the DI Container will identify this descriptor and substitute the corresponding implementation in its place.
```cpp
template <
    typename TEngine = EngineDescriptor,
    typename TTransmission = TransmissionDescriptor>
class FordMustang {
    ...
```
_**Hint**: if you want to use autocompletion when working with dependencies, and C++20 and concepts are not available to you, you can declare the same methods and fields in the descriptor as in the implementation._
```cpp
struct EngineDescriptor {
    void Start();
    void IncreaseRate();
    void DecreaseRate();
    void ShutDown();
};
```
</details>

## Step 3. Link the descriptor to the implementation
To specify which implementation should be substituted for the descriptor, it is necessary to declare a **Bind** structure specification for this descriptor.
```cpp
template <>
struct IOC::Binding<EngineDescriptor> {
    using TService = V4EcoBoost;
};

template <>
struct IOC::Binding<TransmissionDescriptor> {
    using TService = Automatic10Speed;
};
```

Now, if you ever get bored and decide to increase your Mustang's engine displacement to 5 liters, it will only take you one line of code:
```cpp
template <>
struct IOC::Binding<EngineDescriptor> {
    using TService = V8;
};
```

## Step 4. Specify Lifetime
There are 3 types of object lifetimes.

1. **Transient** - every time you request such an object, the container creates a new one. It is used rarely, mostly in situations where you wouldn't want to explicitly create an object through a direct constructor call for some reason (for example, the object is created in multiple places in the program and has a non-trivial set of constructor arguments).

2. **Singleton** - the same object is used every time it's requested. The object is created on the first request. It's used when the type has no state (stateless) or the state needs to be accessible throughout the program's lifetime, and recreating it is undesirable, for instance, a database connection.

3. **Scoped** - similar to Singleton, but the uniqueness and lifetime of the object are limited to the scope within which it's created. For example, it could be a local cache limited to the lifespan of a single HTTP request.

The container manages the ownership of _Scoped_ and _Singleton_ objects, so for these objects, the container will return a **pointer**. In the case of _Transient_ lifetime, the container will create and return the **complete object**.

For the engine and transmission, we will specify Transient lifetime - it would be strange if the same engine or transmission were used simultaneously in several different cars.
```cpp
template <>
struct IOC::Binding<EngineDescriptor> {
    using TService = V4EcoBoost;
    using TLifetime = IOC::Transient;
};

template <>
struct IOC::Binding<TransmissionDescriptor> {
    using TService = Automatic10Speed;
    using TLifetime = IOC::Transient;
};
```

At the same time, the same driver can easily choose different cars in their garage. Therefore, we will specify Scoped lifetime for both the driver and the car.
```cpp
template <>
struct IOC::Binding<VehicleDescriptor> {
    using TService = FordMustang<>;
    using TLifetime = IOC::Scoped;
};

template <>
struct IOC::Binding<DriverDescriptor> {
    using TService = Alice<>;
    using TLifetime = IOC::Scoped;
};
```

If we specified Singleton lifetime for them, it would mean that there could only be one instance of `Alice<>` and one `FordMustang<>` in the entire world.

Since cars are stored in the garage (_scope_), they are passed to the `Alice<>` class not by value but by pointer. The same applies to Singletons.
```cpp
template <typename TVehicle = VehicleDescriptor>
class Alice {
private:
    TVehicle* Vehicle_;

public:
    Alice(TVehicle vehicle) : Vehicle_(vehicle) {
    }
...
};
```

## Step 4. Specify custom factory if needed
Let's assume that `FordMustang<>` has an additional parameter - the body style, which is defined in the `EBodyStyle` enum. We can create a factory specification that sets the body style:
```cpp
template<>
struct IOC::ServiceFactory<FordMustang<>> {
    static auto Create(auto& container) {
        using TFullType = IOC::Util::ReplaceDescriptors<FordMustang<>>::TResult;

        return TFullType(
            container.Resolve<EngineDescriptor>(),
            container.Resolve<TransmissionDescriptor>(),
            EModelType::Coupe);
    }
};
```
Why should we use ReplaceDescriptors utility here?
Because the type of the `FordMustang<>` class is `FordMustang<EngineDescriptor, TransmissionDescriptor>`, so we need to replace the descriptors by corresponding real types before creating a car.
The result of calling `ReplaceDescriptors` will be of type `FordMustang<V4EcoBoost, Automatic10Speed>` according to the bindings.

## Step 5. Register types and build Container
Before you can start using the container, you need to register dependencies of your types within it. Registration involves listing all the types whose creation might be requested directly or indirectly by the container when initializing these objects' dependencies.

```cpp
using TVehicleContainer = IOC::Container<
    VehicleDescriptor,
    EngineDescriptor,
    TransmissionDescriptors
>
```

Multiple containers can be combined into one.

```cpp
IOC::Container<
    TVehicleContainer,
    TLocationContainer,
    TDriverContainer
> container;
```

## Step 6. Start using
```cpp
auto* location = container.Resolve<LocationDescriptor>();
auto* driver = container.Resolve<DriverDescriptor>();

driver->DriveTo(location);

static_assert(std::same_as<
    decltype(driver->GetCar()),
    FordMustang<V4EcoBoost, Automatic10Speed>);
```