#include "../include/EventEmitter.hpp"
#include <iostream>

// Testing out with a struct that cannot be copied.
struct NoCopy {
    NoCopy(int i)
        : m_i(i)
    {
    }
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;

    int m_i {};
};

struct Employee {
    void got_paid(int amount)
    {
        std::cout << "Just made $" << amount << "!, time to party!.\n";
    }
};

void print_name(const std::string& name)
{
    std::cout << "My name is: " << name << '\n';
}

void read_no_copy(const NoCopy& nc)
{
    std::cout << "NoCopy is: " << nc.m_i << '\n';
}

void modify_no_copy(NoCopy& nc)
{
    nc.m_i = 69;
    std::cout << "NoCopy is now: " << nc.m_i << '\n';
}

int main()
{
    auto diff = [](int a, int b) { std::cout << "a - b = " << a - b << '\n'; };
    auto sum = [](int a, int b) { std::cout << "a + b = " << a + b << '\n'; };
    auto sum_2 = [](int a, int b) { std::cout << "I don't even sum.\n"; };

    EventEmitter emitter;
    emitter.on("diff", diff);
    emitter.emit("diff", 1, 2);

    emitter.on("sum", sum);
    emitter.on("sum", sum_2);
    emitter.emit("sum", 1, 2);

    Employee e;

    emitter.on("paycheck", [&e](int amount) { e.got_paid(amount); });
    emitter.emit("paycheck", 100);

    NoCopy nc { 42 };
    emitter.on("no_copy_read", read_no_copy);
    emitter.on("no_copy_modify", modify_no_copy);

    // To use a const reference, we need to pass it as a const reference.
    const NoCopy& nc_ref = nc;
    emitter.emit("no_copy_read", nc_ref);
    // If your function takes a non-const reference, you can pass it as a non-const reference.
    emitter.emit("no_copy_modify", nc);

    // Print the nocopy's new value.
    std::cout << "NoCopy is now: " << nc.m_i << '\n';

    emitter.on("name_received", print_name);
    const std::string name = "John Doe";
    emitter.emit("name_received", name);

    int counter {};
    emitter.on("counter++", [](int& c) { c++; });
    emitter.emit("counter++", counter);
    emitter.emit("counter++", counter);

    // Print the counter.
    std::cout << "Counter: " << counter << '\n';
}
