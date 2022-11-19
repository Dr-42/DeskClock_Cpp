# DeskClock

This is a C++ implementation of my previously built DeskClock using C++ and some nicer features(date and colored hands)

### Environment setup

#### Windows

I am using the MSYS2 mingw 64-bit g++ compiler for installation of libraries and such.

#### Dependencies

1. GLEW
2. GLFW
3. Freetype2
4. STB Image
5. mingw64_crt

### Build

To install libraries in msys2

```
pacman -S mingw-w64-glew mingw-w64-glfw mingw-w64-stb mingw-w64-freetype mingw-w64-crt-git
```


After proper installation and configuration of libraries change the game.cpp file's Resource Manager calls to the location where you cloned the repo.

Then run

``` python builder.py run ```

To clean build, run

``` python builder.py clean ```

To build only run

``` python builder.py ```
The binaries will be placed in the bin directory
