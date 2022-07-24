
# EventEmitter

A header-only event emitter implementation written in C++ 20.


## Usage/Examples

```cpp
#include "EventEmitter.hpp"

auto diff = [](int a, int b) { std::cout << "a - b = " << a - b << '\n'; };

EventEmitter emitter;
emitter.on("diff", diff);
emitter.emit("diff", 1, 2);

// prints: a - b = -1
```


## API Reference

#### on("event_name", on_event)

Adds a listener to the emitter, listening for the event specified. Essentially adds the string to the map and adds the functor to a list of functors to run when the event is emitted.

```cpp
  emitter.on("login_event", on_login);
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `event_name` | `string_view` | **Required**. The event name to listen for. |
| `callback` | `Event` | **Required**. The actual callable function to run when at the time of the event. |

#### emit("event_name", args...)

Emits the event to the list of listeners, calling them all with the arguments provided.

```cpp
  emitter.emit("login_event", user_name, timestamp);
```

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `event_name`      | `string_view` | **Required**. The event to emit to the list of listeners. |
| `args`      | `Args...` | The arguments to pass to the listening functions. |

