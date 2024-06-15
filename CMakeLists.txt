cmake_minimum_required(VERSION 3.24)
project(ip_trie C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity)

add_library(ip_trie_lib OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Trie.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ip.c
)

target_include_directories(ip_trie_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(ip_trie main.c)
target_link_libraries(ip_trie
        ip_trie_lib
)

include(CTest)

add_executable(trie_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/trie_test.c)
add_executable(ip_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/ip_test.c)


target_link_libraries(trie_test PUBLIC
        ip_trie_lib
        unity
)

target_link_libraries(ip_test PUBLIC
        ip_trie_lib
        unity
)

add_test(NAME trie_test COMMAND trie_test)
add_test(NAME ip_test COMMAND ip_test)

add_custom_target(check
        COMMAND trie_test ip_test
        DEPENDS trie_test ip_test
)
