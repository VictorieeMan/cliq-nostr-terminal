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