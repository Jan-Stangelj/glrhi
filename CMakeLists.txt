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
add_executable("Renderer" "${SOURCES}")

target_compile_definitions("Renderer" PUBLIC "GLFW_INCLUDE_NONE=1") 

target_include_directories("Renderer" PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories("Renderer" PRIVATE  ${PROJECT_SOURCE_DIR}/vendor/include)

target_link_libraries("Renderer" glad glfw glm)