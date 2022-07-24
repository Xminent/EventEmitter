#pragma once
#include <functional>
#include <tuple>

// Credit to: https://stackoverflow.com/a/21665705

// For generic types that are functors, delegate to its 'operator()'
template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())> {
};

// for pointers to member function
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const> {
    using f_type = std::function<ReturnType(Args...)>;
    using arg_types = std::tuple<Args...>;
};

// for pointers to member function
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...)> {
    using f_type = std::function<ReturnType(Args...)>;
    using arg_types = std::tuple<Args...>;
};

// for function pointers
template <typename ReturnType, typename... Args>
struct function_traits<ReturnType (*)(Args...)> {
    using f_type = std::function<ReturnType(Args...)>;
    using arg_types = std::tuple<Args...>;
};

template <typename L>
using f_type_t = typename function_traits<L>::f_type;

template <typename L>
using arg_types_t = typename function_traits<L>::arg_types;
