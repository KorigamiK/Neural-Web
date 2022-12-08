project(neural-web-test LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/simple.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE neural-web)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
