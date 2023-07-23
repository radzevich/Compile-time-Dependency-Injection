#pragma once

#include <experimental/coroutine>
#include <type_traits>
#include <utility>
#include <exception>
#include <iterator>
#include <functional>

namespace Example::Util {

    /*
     * Provides a convenient way to implement coroutines that yield values,
     * allowing them to be used with range-based for-loops and standard algorithms that accept iterators and ranges.
     * The Enumerable class is intended to be used as the return type for coroutines that produce a sequence of values.
     * It acts as the return object and is tightly coupled with the Promise type.
     * The Promise and Iterator components work together to manage the coroutine state and expose the yielded values
     * to the client code.
     * */
    template <typename T>
    class Enumerable {
        /*
         * Implements the coroutine promise type.
         * Responsible for managing the coroutine state and providing the necessary functions and behavior for the coroutine.
        */
        struct Promise {
            T value_;

            auto get_return_object() -> Enumerable {
                using Handle = std::experimental::coroutine_handle<Promise>;
                return Enumerable{Handle::from_promise(*this)};
            }

            auto initial_suspend() {
                return std::experimental::suspend_always(); // coroutine is suspended from the very beginning
            }

            auto final_suspend() noexcept {
                return std::experimental::suspend_always(); // coroutine is suspended in the end of execution
            }

            void return_void() {
            }

            void unhandled_exception() {
                throw;
            }

            auto yield_value(T&& value) {
                value_ = std::move(value);
                return std::experimental::suspend_always();
            }

            auto yield_value(const T& value) {
                value_ = value;
                return std::experimental::suspend_always();
            }
        };

        struct Sentinel {};

        /*
         * Serves as the interface between the client code and the Promise.
         * It implements the input iterator concept and provides the necessary functions and operators to iterate over the yielded values.
        */
        struct Iterator {
            using iterator_category = std::input_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            std::experimental::coroutine_handle<Promise> Handle_;

            Iterator& operator++() {
                Handle_.resume();
                return *this;
            }

            void operator++(int) {
                (void)operator++();
            }

            T operator*() const {
                return Handle_.promise().value_;
            }

            T* operator->() const {
                return std::addressof(operator*());
            }

            bool operator==(Sentinel) const {
                return Handle_.done();
            }
        };

        std::experimental::coroutine_handle<Promise> Handle_;

        explicit Enumerable(std::experimental::coroutine_handle<Promise> handle) : Handle_(handle) {
        }

    public:
        using promise_type = Promise;

        Enumerable(Enumerable&& g) noexcept : Handle_(std::exchange(g.Handle_, {})) {
        }

        ~Enumerable() {
            if (Handle_) {
                Handle_.destroy();
            }
        }

        auto begin() {
            Handle_.resume();
            return Iterator{Handle_};
        }

        auto end() {
            return Sentinel{};
        }
    };

}