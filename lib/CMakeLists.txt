project(
    neural-web 
    VERSION 0.1.0
    DESCRIPTION "Neural Web Library"
    LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

message(STATUS "Lib Sources: ${SOURCES}")
message(STATUS "Lib Headers: ${HEADERS}")

add_library( ${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS} )

target_include_directories( ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if (BUILD_TESTING)
    add_subdirectory(test)
endif()