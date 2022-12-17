project(
    visualizer
    VERSION 0.1.0
    DESCRIPTION "Visualizer"
    LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# make symbolic link to res folder in the build directory
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    # preload res folder
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file res")
endif()

message(STATUS "Visualizer Sources: ${SOURCES}")
message(STATUS "Visualizer Headers: ${HEADERS}")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE
    neural-web
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_GFX_LIBRARIES}
		${SDL2_TTF_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
		${SDL2_TTF_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

