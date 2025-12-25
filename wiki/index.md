## TODO :

- [ ] Figure out doxygen setup for documentation

- [X] implement uuid

- [ ] Figure out licence stuff (and properly acknowledge deps) [used libs](used libs)

- [.] the functions and variables should be renamed and documented properly.

- [.] Think about a potential array of custom callbacks identified by name
         > Could be used like `run_callbacks_recursive(&root, "callback_name")`

         > EDIT: I implemented
         > `entity_foreach_children(&root, depth, void* (*function)(*entity, depth))`
         > which is more or less equivalent and more flexible.
         > The user can implement their own registry.
         > You could even filter inside the callback itself based on some property of the entity.
         > Seems powrerful, but maybe not as user-friendly and could be less efficient.

- [ ] Idem for custom properties (a dynamic array of props identified by a string with getter and setter functions)
         > This seems actually harder to implement than the callbacks, how would we handle types? Maybe void* with size and user casts? How would ffi handle that?
         > tbd.

- [ ] Figure out a way to handle referential for the position of child entities
         > Maybe have a flag to indicate if the position is in relation to the parent ? What if the parent doesn't have a position ?
         > We could force the root node to have a position, and recursively traverse the tree backwards to compute the absolute position, that way not all entities need to have a position.
         > Seems dumb for logic entities that don't need a position, I don't want my users to think that the api is only for graphical entities.
         > Defaulting to 0,0 could be an option too. But should 0 represent the center or the top-left corner ? In absolute positioning top-left makes sense, but in relative positioning center is better.
         > The issue is that 0,0 centered for root is screen-size dependent, and we should not tightly couple the entity system to rendering.

- [ ] Add a tag system to entities for searching/filtering
         > A simple array of strings maybe sufficient.

- [ ] Implement a debug tree printer (a la unix tree command).

- [ ] Maybe useful to have a way to dump the tree to json or yaml, and load it back.
         > Cool for level design, but very weird (or even impossible ?) to serialize callbacks.
         > Might work in python, but i want everything to work in C too.
         > Could potentially abuse dlopen and dlsym to load callbacks by symbol name at runtime, but I want the build process to be as simple as possible.
         > There is another option, we could do codegen at build time but is it worth the complexity? Having to write tree descriptions in another format is
         > arguably not a better experience than writing code directly. (but it allows tools to generate levels)
         > Maybe just provide the serialization and let users figure out how to handle callbacks themselves (with codegen, dlopen or python bindings).

## DONE :

- [X] entity_create() should return the struct by value, not allocate it
         > This is dumb to malloc from there because the user should handle the lifetime of only root entities, and could do so on the stack
         > the child entities are passed by value anyways and owned by the parents.

[GPT summary](GPT summary)
