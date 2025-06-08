cmake_minimum_required(VERSION 3.16)

project("Renderer" LANGUAGES CXX C)
set (CMAKE_CXX_STANDARD 20)

set("GLFW_BUILD_DOCS" OFF CACHE BOOL "" FORCE)
set("GLFW_BUILD_TESTS" OFF CACHE BOOL "" FORCE)
set("GLFW_BUILD_EXAMPLES" OFF CACHE BOOL "" FORCE)
set("GLFW_INSTALL" OFF CACHE BOOL "" FORCE)
set("BUILD_SHARED_LIBS" OFF CACHE BOOL "" FORCE)

add_subdirectory("vendor/glad")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")

file(GLOB_RECURSE "SOURCES" CONFIGURE_DEPENDS "src/*.cpp")
add_library("glrhi" "${SOURCES}")

target_compile_definitions("glrhi" PUBLIC "GLFW_INCLUDE_NONE=1") 

target_include_directories("glrhi" PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories("glrhi" PUBLIC  ${PROJECT_SOURCE_DIR}/vendor/include)

target_link_libraries("glrhi" PUBLIC glad glfw glm)

add_executable("triangle" "examples/triangle/src/main.cpp")
target_link_libraries("triangle" glrhi)

add_executable("texture" "examples/texture/src/main.cpp")
target_link_libraries("texture" glrhi)

add_executable("framebuffer" "examples/framebuffer/src/main.cpp")
target_link_libraries("framebuffer" glrhi)