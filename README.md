# glrhi

## Introduction
glrhi (Open**GL** **R**endering **H**ardware **I**nterface) is a library aimed
at abstracting Modern OpenGL 4.6 using modern concepts, like DSA (Direct state acces) to reduce boilerplate at no preformance overhead.

## Building

### Linux
Requirements:
- CMake 3.16
- A C++ 20 compiler
- GNU Make
- A GPU supporting OpenGL 4.6 (not required for building, but for using the library)

Build process:

First clone the repo.

        git clone --recursive https://github.com/Jan-Stangelj/glrhi.git

Then make the build directory and cd to it.

        cd glrhi
        mkdir build
        cd build

Configure the project

        cmake ..

And compile it (The j option sets the amount of threads make will use)

        make -j 4

### Windows
**TBD**

## Usage
To use the library in a CMake project, first add it as a subdirectory.

        add_subdirectory("vendor/glrhi)

(Optional) Disable building examples

        set("GLRHI_BUILD_EXAMPLES" OFF CACHE BOOL "")
        add_subdirectory("vendor/glrhi)

Finally link the library to your executable.

        target_link_libraries("your_executable" glrhi)

## Examples
See [examples](EXAMPLES.md).