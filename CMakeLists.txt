cmake_minimum_required(VERSION 3.14.0)

project(
    neural-web 
    VERSION 0.1.0
    DESCRIPTION "Neural Web"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/visualizer/*.hpp")
file(GLOB_RECURSE LIBS "lib/*.cpp")

message(STATUS "Sources: ${SOURCES}")
message(STATUS "Headers: ${HEADERS}")
message(STATUS "Libs: ${LIBS}")

add_executable(visualizer)

add_library(neural-web SHARED ${LIBS})
target_include_directories(neural-web PUBLIC include/lib)

target_link_libraries(visualizer PRIVATE neural-web)

target_include_directories(visualizer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(visualizer PRIVATE 
    ${SOURCES}
    ${HEADERS}
)


include(CTest)
enable_testing()

if (BUILD_TESTING)
    add_subdirectory(test)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
