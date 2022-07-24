#pragma once
#include <function_traits.hpp>
#include <memory>
#include <typeindex>

/// Represents a listener for a specific event.
class BaseEventListener {
public:
    BaseEventListener(const BaseEventListener&) = delete;
    BaseEventListener& operator=(const BaseEventListener&) = delete;
    BaseEventListener(BaseEventListener&&) = default;
    BaseEventListener& operator=(BaseEventListener&&) = default;
    virtual ~BaseEventListener() = default;

    /// Hint to our underlying type.
    [[nodiscard]] virtual std::type_index t_index() const = 0;
};

template <typename T>
class EventListener;

template <typename... Args>
class EventListener<std::tuple<Args...>> : public BaseEventListener {
public:
    explicit EventListener(std::function<void(Args...)> callback)
        : m_callback { std::move(callback) }
    {
    }

    template <typename... SameArgs>
    requires(std::is_convertible_v<SameArgs, Args>&&...) void operator()(SameArgs&&... args)
    {
        m_callback(std::forward<SameArgs>(args)...);
    }

    [[nodiscard]] std::type_index t_index() const override
    {
        return typeid(std::tuple<Args...>);
    }

private:
    std::function<void(Args...)> m_callback;
};

/// EventEmitter implementation. Uses strings for mapping event names to callbacks which can vary in type. This is a strictly-typed event system in which the listener's parameter type must be the same as the emitter's argument type. For example, if your listener takes a const int& as a parameter, you must emit a const int& as an argument.
class EventEmitter {
public:
    template <typename Event>
    void on(std::string_view event_name, Event callback)
    {
        auto listener = std::make_unique<EventListener<arg_types_t<Event>>>(callback);

        if (auto it = m_listeners.find(event_name); it != m_listeners.end()) {
            return it->second.push_back(std::move(listener));
        }
        auto [v_it, _] = m_listeners.try_emplace(std::string(event_name) /* emplacing a default constructed object */);
        v_it->second.push_back(std::move(listener));
    }

    template <typename... Args>
    void emit(std::string_view event_name, Args&&... args)
    {
        auto it = m_listeners.find(event_name);
        if (it == m_listeners.end()) {
            return;
        }

        for (const auto& listener : it->second) {
            // We need to compare the type_index of our args with the type_index of our listener.
            // This has to be done because an obvious mismatch in the types can result in undefined behavior.
            if (listener->t_index() == typeid(std::tuple<Args...>)) {
                // Call the listener with the arguments.
                static_cast<EventListener<std::tuple<Args...>>*>(listener.get())->operator()(std::forward<Args>(args)...);
            }
        }
    }

private:
    struct StringHash {
        using hash_type = std::hash<std::string_view>;
        using is_transparent = void;

        size_t operator()(const char* str) const { return hash_type {}(str); }
        size_t operator()(std::string_view str) const { return hash_type {}(str); }
        size_t operator()(std::string const& str) const { return hash_type {}(str); }
    };

    /// Map which will contain listeners for each event.
    std::unordered_map<std::string, std::vector<std::unique_ptr<BaseEventListener>>, StringHash, std::equal_to<>> m_listeners {};
};
