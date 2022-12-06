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
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

add_executable(visualizer)

add_library(neural-web STATIC ${LIBS})
target_include_directories(neural-web PUBLIC include/lib)

target_sources(visualizer PRIVATE 
    ${SOURCES}
    ${HEADERS}
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    message(STATUS "Emscripten detected")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FULL_ES2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(SDL2_LIBRARIES "-s USE_SDL=2")
    set(SDL2_IMAGE_LIBRARIES "-s USE_SDL_IMAGE=2")
else()
    message(STATUS "Native build detected")
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    message(STATUS "SDL2: ${SDL2_LIBRARIES} ${SDL2_INCLUDE_DIRS}")
    message(STATUS "SDL2_image: ${SDL2_IMAGE_LIBRARIES} ${SDL2_IMAGE_INCLUDE_DIRS}")
    enable_testing()
endif()

target_link_libraries(visualizer PRIVATE 
    neural-web
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

target_include_directories(visualizer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

include(CTest)

if (BUILD_TESTING)
    add_subdirectory(test)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
