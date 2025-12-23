## Project Scope and Goals:

### Objective:

- Build a lightweight, callback-driven game development library on top of Raylib for quick prototyping.
- Avoid unnecessary complexity (no GUI, no imposed main loop) and keep the API simple, flexible, and user-friendly.
- Focus on extending Raylib’s strengths for entity-based systems with an emphasis on usability.

### Core Features:

- Entity System:
   - A struct-based system (Entity) with callbacks (on_update, on_draw) for behavior.
   - Hierarchical, parent-child relationships (entities own their children; everything auto-managed).
   - Calls like entity_create, entity_add_child, entity_destroy.

- Lightweight Main Loop Support:
   - Example functions for update and draw with the ability to traverse and manage entities.
   - Optionally let users plug their own main loop behavior.

- FFI Python Bindings:
   - Expose the library to Python via ctypes or cffi.
   - Allow users to define Python callbacks for entities (on_update/on_draw).
   - Memory management handled seamlessly with Python object wrappers (C allocates/deallocates behind the scenes).

- Simplicity Over Features:
   - Avoid dictating game architecture. Let users structure things themselves.

- Stretch Features (Future Ideas):
   - Scene graph for organized rendering and updates.
   - Optional lightweight physics system (using Box2D/Chipmunk2D).
   - Events system for cleaner inter-entity communication.

## Action Plan:

- Design Core C Library:
   - Implement the Entity system:
      - Create/integrate the Entity struct (with children, callbacks, and user-defined data support).
      - Functions: entity_create, entity_add_child, entity_for_each_recursive, entity_destroy, etc.
      - Allow callback-driven behavior for updates/drawing.

- Expose Python FFI Bindings:
   -Use ctypes or cffi to wrap the core C functions.
   - Ensure Python objects manage memory automatically via finalizers (__del__) to call entity_destroy.

- Create Examples for Use:

- Include basic demos (main loop system, entity spawning, 2D rendering, and callback behaviors).
- Provide Python examples (e.g., defining custom on_update/on_draw for entities).

- Document and Polish:
   - Write concise API documentation.
   - Clearly describe memory management principles for both C and Python users.
