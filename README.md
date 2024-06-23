# IP-Trie

This project is a Trie-based IP address management system implemented in C. It provides an efficient way to store and
manage IP addresses and their associated subnet masks.

## Features

- **Add IP Address**: The `add` function allows you to add an IP address and its associated subnet mask to the data
  structure.
- **Delete IP Address**: The `del` function allows you to remove an IP address and its associated subnet mask from data
  structure.
- **Check IP Address**: The `check` function checks if an IP address is present in the data structure and returns its
  associated subnet mask.
- **Test Suite**: The project includes a comprehensive test suite written with the Unity test framework. The tests cover
  all major functionalities of the data structure.

## Getting Started

To get started with this project, clone the repository and build the project using your preferred C compiler.

```bash
    git clone https://github.com/jlatusek/ip-trie
    cd ip-trie
    cmake \
      -Bbuild \
      -DCMAKE_BUILD_TYPE=Release \
      -G Ninja
    cmake --build build
```

## Running Tests

The tests for this project are written using the Unity test framework. To run the tests, compile the project and in
build directory run:

```bash
    cd build
    ctest
```

### Coverage

To generate a coverage report, you can run the following commands:

```bash
    cmake --build build --target coverage
```

### Cppcheck

To run the cppcheck static analysis tool, you can run the following commands:

```bash
    cmake --build build --target cppcheck
```

Result report will be saved in `build/cppcheck/index.html`.

## License

This project is licensed under the MIT License.