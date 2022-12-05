set(TEST_PROJECT "neural-web-test")
project(${TEST_PROJECT} LANGUAGES CXX)

add_executable(${TEST_PROJECT} src/simple.cpp)
target_link_libraries(${TEST_PROJECT} PRIVATE neural-web)

target_include_directories(${TEST_PROJECT} PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME ${TEST_PROJECT} COMMAND ${TEST_PROJECT})
