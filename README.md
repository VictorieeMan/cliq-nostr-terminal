# cliq-nostr-terminal
A client that makes you feel like you are part of an exclusive clique of nostr users who prefer the terminal.

```
git clone https://github.com/VictorieeMan/cliq-nostr-terminal.git
```

## Dependencies

- [libwebsockets](https://libwebsockets.org/) minimum [v4.3.2]()
  - Install instructions: https://libwebsockets.org/lws-api-doc-v4.3-stable/html/md_READMEs_README_build.html
  - Using vcpkg: `vcpkg install libwebsockets`
  <!-- - Remember to customize CMake to inform the linker where to find the library.
  ```CMake
  # Including external libraries
  find_package(libwebsockets CONFIG REQUIRED)
  target_link_libraries(Cliq PRIVATE websockets_shared)
  ``` -->
- [simdjson](https://github.com/simdjson/simdjson) minimum [v3.2.1](https://github.com/simdjson/simdjson/releases/tag/v3.2.1)

## Windows
Install vcpkg and the dependencies using the following commands:
```
git clone https://github.com/microsoft/vcpkg.git;
cd vcpkg;
./bootstrap-vcpkg.sh;
./vcpkg integrate install
```

```
vcpkg install libwebsockets
vcpkg install simdjson
```