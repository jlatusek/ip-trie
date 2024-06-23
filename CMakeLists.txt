cmake_minimum_required(VERSION 3.24)
project(ip_trie C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic --coverage")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity)

add_library(ip_trie_lib OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Trie.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ip.c
        src/ip_tool.c
)

target_compile_options(ip_trie_lib PRIVATE
        --coverage
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
add_executable(tool_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/tool_test.c)
add_executable(ip_test_not_init ${CMAKE_CURRENT_SOURCE_DIR}/tests/ip_test_not_initialized.c)


target_link_libraries(trie_test PUBLIC
        ip_trie_lib
        unity
)

target_link_libraries(ip_test PUBLIC
        ip_trie_lib
        unity
)

target_link_libraries(tool_test PUBLIC
        ip_trie_lib
        unity
)

target_link_libraries(ip_test_not_init PUBLIC
        ip_trie_lib
        unity
)

add_test(NAME trie_test COMMAND trie_test)
add_test(NAME ip_test COMMAND ip_test)
add_test(NAME tool_test COMMAND tool_test)
add_test(NAME ip_test_not_init COMMAND ip_test_not_init)
SET(TESTS
        trie_test
        ip_test
        tool_test
        ip_test_not_init
)

add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND lcov --capture --directory . --output-file coverage.info
        COMMAND lcov --remove coverage.info 'third_party/Unity/*' --output-file coverage.info
        COMMAND genhtml --branch-coverage coverage.info --output-directory coverage
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(cccc
        COMMAND cccc src/*.c include/*.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(cppcheck
        COMMAND cppcheck --enable=all --inconclusive --xml --xml-version=2 --force --language=c --std=c23 --platform=unix64
        --output-file=${CMAKE_CURRENT_BINARY_DIR}/cppcheck.xml ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/tests
        COMMAND cppcheck-htmlreport --file=${CMAKE_CURRENT_BINARY_DIR}/cppcheck.xml --report-dir=${CMAKE_CURRENT_BINARY_DIR}/cppcheck
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(valgrind
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ${CMAKE_CURRENT_BINARY_DIR}/trie_test > /dev/null
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ${CMAKE_CURRENT_BINARY_DIR}/ip_test > /dev/null
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ${CMAKE_CURRENT_BINARY_DIR}/tool_test > /dev/null
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ${CMAKE_CURRENT_BINARY_DIR}/ip_test_not_init > /dev/null
        DEPENDS ${TESTS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS ${TESTS}
)