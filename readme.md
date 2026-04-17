## TODOs

- [x] ~~move the scene into a view~~
- [ ] hud colors
- [x] bring the radar widget
- [ ] complete radar items
- [ ] bring the clouds effect into scene view
- [ ] bring the sound into scene view
- [ ] complete the debug view by type
- [ ] bring the autopilot controller
- [ ] check safe landing mechanism
- [ ] bring scene sounds
- [ ] bring the navball widget
- [ ] complete screens
- [ ] complete crash layout
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