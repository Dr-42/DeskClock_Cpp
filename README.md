# Nomu_Engine

This is a basic Rendering engine using opengl. Most of it learned from https://learnopengl.com.

## Features

This Engine copiles and works on both Windows and Linux. This is made possible by my own build tool CppBuild. I never got the hang of cmake and I don't knnow why my  Makefiles werent very satisfying. So this is the result.

## Usage

### Environment setup

#### Windows

I am using the MSYS2 mingw 64-bit g++ compiler for installation of libraries and such.

#### Linux

I have tested this both on my wsl and physical laptop.

#### Dependencies

1. GLEW
2. GLFW
3. Freetype2
4. STB Image

### Build

After proper installation and configuration of libraries, you can run

``` python builder.py run ```

To clean build, run

``` python builder.py clean ```

To build only run

``` python builder.py ```
The binaries will be placed in the bin directory
