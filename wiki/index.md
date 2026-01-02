## TODO :

- [ ] Figure out doxygen setup for documentation


- [ ] Figure out licence stuff (and properly acknowledge deps) [used libs](used libs)

- [o] the functions and variables should be renamed and documented properly.
   - [ ] figure out what needs to be public api and what should be private.

- [.] Think about a potential array of custom callbacks identified by name
         > Could be used like `run_callbacks_recursive(&root, "callback_name")`
         > EDIT: I implemented
         > `entity_foreach_children(&root, depth, void* (*function)(*entity, depth))`
         > which is more or less equivalent and more flexible.
         > The user can implement their own registry.
         > You could even filter inside the callback itself based on some property of the entity.
         > Seems powrerful, but maybe not as user-friendly and could be less efficient.


- [ ] Add a tag system to entities for searching/filtering
         > A simple array of strings maybe sufficient.


- [ ] Implement a json reader, the writer is more or less done.
         > Just need to implement custom serialization for data pointed by user_data.


## DONE :

- [X] entity_create() should return the struct by value, not allocate it
         > This is dumb to malloc from there because the user should handle the lifetime of only root entities, and could do so on the stack
         > the child entities are passed by value anyways and owned by the parents.

- [X] Implement a debug tree printer (a la unix tree command).
         > We can now either have a tree-like debug printer or dump a json.

- [X] Figure out a way to handle referential for the position of child entities
         > Maybe have a flag to indicate if the position is in relation to the parent ? What if the parent doesn't have a position ?
         > We could force the root node to have a position, and recursively traverse the tree backwards to compute the absolute position, that way not all entities need to have a position.
         > DONE: Implemented a `entity_relative_position()` function that traverses the tree upwards to compute the position, seems sufficient.

- [X] Maybe useful to have a way to dump the tree to json or yaml, and load it back.
         > Cool for level design, but very weird (or even impossible ?) to serialize callbacks.
         > Might work in python, but i want everything to work in C too.
         > Could potentially abuse dlopen and dlsym to load callbacks by symbol name at runtime, but I want the build process to be as simple as possible.
         > There is another option, we could do codegen at build time but is it worth the complexity? Having to write tree descriptions in another format is
         > arguably not a better experience than writing code directly. (but it allows tools to generate levels)
         > Maybe just provide the serialization and let users figure out how to handle callbacks themselves (with codegen, dlopen or python bindings).

- [X] Idem for custom properties (a dynamic array of props identified by a string with getter and setter functions)
         > This seems actually harder to implement than the callbacks, how would we handle types? Maybe void* with size and user casts? How would ffi handle that?
         > tbd.
         > DONE: added a `void* user_data` and an optionnal destructor called when `destroy_children`.

- [X] implement uuid

[GPT summary](GPT summary)
