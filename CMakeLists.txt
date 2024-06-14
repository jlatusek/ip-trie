cmake_minimum_required(VERSION 3.20)
project(ip_trie C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpputest)

add_library(ip_trie_lib OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Trie.c
)

target_include_directories(ip_trie_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(ip_trie main.c)
target_link_libraries(ip_trie
        ip_trie_lib
)

include(CTest)

add_executable(ip_trie_test
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/trie_test.cpp
)

target_link_libraries(ip_trie_test PUBLIC
        ip_trie_lib
        CppUTest::CppUTest
        CppUTest::CppUTestExt
)

add_test(NAME ip_trie_test COMMAND ip_trie_test)

