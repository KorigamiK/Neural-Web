cmake_minimum_required(VERSION 3.16.0)

project(neural-web-visualization VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    message(STATUS "Emscripten detected")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(SDL2_LIBRARIES "-s USE_SDL=2")
    set(SDL2_IMAGE_LIBRARIES "-s USE_SDL_IMAGE=2")
else()
    message(STATUS "Native build detected")
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    message(STATUS "SDL2 - ${SDL2_LIBRARIES} ${SDL2_INCLUDE_DIRS}")
    message(STATUS "SDL2_image - ${SDL2_IMAGE_LIBRARIES} ${SDL2_IMAGE_INCLUDE_DIRS}")
    enable_testing()
endif()

add_subdirectory(lib)
add_subdirectory(visualizer)

include(CTest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
