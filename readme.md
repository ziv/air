## TODOs

- [x] ~~move the scene into a view~~
- [x] hud colors
- [x] bring the radar widget
- [ ] complete radar items
- [x] bring the clouds effect into scene view
- [x] bring the sound into scene view
- [ ] complete the debug view by type
- [ ] bring the autopilot controller
- [ ] check safe landing mechanism
- [x] bring scene sounds
- [ ] bring the navball widget
- [ ] complete screens
- [x] complete crash layout
- [ ] refactor scene data into a scene config
-

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

Style Rules

- Use `PascalCase` for class names, structs, and enums
- Use `kebab_case` for variables, methods, and functions
- Systems functions should be named using `PascalCase` (see main rendering loop in the game)


```shell
cmake -B cmake-build-debug -S . -DCMAKE_BUILD_TYPE=Debug -G Ninja
```