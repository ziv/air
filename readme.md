## Modules Rules

```c++
module;
// includes only here:
#include <vector>

export module MyModule;

// import only here:
import JsonConfig;

export class MyClass { ... };
```

## Modern CPP Rules

- Modules
- `static_cast` for casting
- `if-init` statements
- `auto` for type inference
- `constexpr` for compile-time constants
- `nullptr` instead of `NULL`