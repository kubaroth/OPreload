A custom HDK operator example with an option to reload changes at runtime, to retain the state of the loaded scene.
- Updates to GU_Detail are supported.
- Changes to existing parameters should work as well.
- Adding new parameters is currently problematic since they are expected to be declared as static.

[![demo1 - workflow](./images/demo1.png)](./images/demo1.mp4)

## Build:
Update the CMakeLists.txt to point to Houdini installation directory ($HFS).
The `make install` copies the host shared library to the dso folder inside $HOME/houdini18.0.
```
mkdir __build
cd __build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/houdini18.0/dso
make install
```

## NOTES:
Houdini OP_OperatorTable - does not handle reloading previously loaded dsos. There some existing references being created during plugin registration (HOM etc) and there is no API (that I know of) to handle this. 
To workaround this, on every new build create new dso file keeping the plugin internal names unchanged. Loading a new dso containing same symbols should override and update existing operator (OP_Operator). 
To update an instance of a node in the scene - delete the node and undo the operation- which should pick up changes. 
NOTE: The reload is currently associated with the scene reload callback. (see the linked demo1.mp4)

## TODO:
- move all the globals into VersionCounter singleton (Done)
- handle cleanup of generated intermediate dsos
- extract base_name of the file (try to avoid Boost filesystem - missing symbols in shipped version of hboost)
