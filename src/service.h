#pragma once

#include <iostream>

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