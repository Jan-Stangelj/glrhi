cmake_minimum_required(VERSION 3.16)

project("glrhi" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set("GLFW_BUILD_DOCS" OFF CACHE BOOL "" FORCE)
set("GLFW_BUILD_TESTS" OFF CACHE BOOL "" FORCE)
set("GLFW_BUILD_EXAMPLES" OFF CACHE BOOL "" FORCE)
set("GLFW_INSTALL" OFF CACHE BOOL "" FORCE)
set("BUILD_SHARED_LIBS" OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)

set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER OFF)
set(ASSIMP_BUILD_3DS_IMPORTER OFF)

add_subdirectory("vendor/glad")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")
add_subdirectory("vendor/assimp")

file(GLOB_RECURSE "SOURCES" CONFIGURE_DEPENDS "src/*.cpp")
add_library("glrhi" "${SOURCES}")

# Enable ALL warnings
if(MSVC)
  target_compile_options(glrhi PRIVATE /W4 /WX)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNUc")
  target_compile_options(glrhi PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(glrhi PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions("glrhi" PUBLIC "GLFW_INCLUDE_NONE=1") 

target_include_directories("glrhi" PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories("glrhi" PUBLIC  ${PROJECT_SOURCE_DIR}/vendor/include)

target_link_libraries("glrhi" PUBLIC glad glfw glm)

option("GLRHI_BUILD_EXAMPLES" "Build examples for glrhi" ON)

if (GLRHI_BUILD_EXAMPLES)
    add_executable("triangle" "examples/triangle/src/main.cpp")
    target_link_libraries("triangle" glrhi)

    add_executable("texture" "examples/texture/src/main.cpp")
    target_link_libraries("texture" glrhi)

    add_executable("framebuffer" "examples/framebuffer/src/main.cpp")
    target_link_libraries("framebuffer" glrhi)

    add_executable("uniform_buffer" "examples/uniform_buffer/src/main.cpp")
    target_link_libraries("uniform_buffer" glrhi)

    add_executable("ssbo" "examples/ssbo/src/main.cpp")
    target_link_libraries("ssbo" glrhi)

    add_executable("compute" "examples/compute/src/main.cpp")
    target_link_libraries("compute" glrhi)

    add_executable("mesh" "examples/mesh/src/main.cpp")
    target_link_libraries("mesh" glrhi)

    add_executable("camera" "examples/camera/src/main.cpp")
    target_link_libraries("camera" glrhi)

    add_executable("material" "examples/material/src/main.cpp")
    target_link_libraries("material" glrhi)
endif()