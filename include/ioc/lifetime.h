#pragma once

namespace IOC {

struct Singleton {};

struct Scoped {};

struct Transient {};

// External: the instance is owned outside of the container and is supplied
// at runtime via `Container::Bind<TDescriptor>(ptr)`. Resolving an External
// descriptor returns a non-owning pointer to that externally-managed object.
// This is the bridge between the compile-time DI graph and runtime values
// (configuration, OS resources, pre-existing collaborators).
struct External {};

} // namespace IOC
