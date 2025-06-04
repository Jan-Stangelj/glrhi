cmake_minimum_required(VERSION 3.16)

project("glad")

add_library("glad")
target_sources("glad" PRIVATE ${PROJECT_SOURCE_DIR}/src/glad.c)
target_include_directories("glad" PUBLIC ${PROJECT_SOURCE_DIR}/include)