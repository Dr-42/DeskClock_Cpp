# DeskClock

This is a C++ implementation of my previously built DeskClock using C++ and some nicer features(date and colored hands)

It is currently configured for 1920 x 1080 resolution for now.

## Install
--------------------------------------------------------------

You can build the software yourself or just download the setup file provided.

## Build
--------------------------------------------------------------

This project uses a Rust based build tool called builder_cpp

### Environment setup

I am using the MSYS2 mingw 64-bit g++ compiler for installation of libraries and such.

#### Dependencies

1. GLEW
2. GLFW
3. Freetype2
4. STB Image
5. mingw64_crt

To install libraries in msys2

```
pacman -S mingw-w64-x86_64-glew mingw-w64-x86_64-glfw mingw-w64-x86_64-stb mingw-w64-x86_64-freetype mingw-w64-x86_64-crt-git
```

### To build
```
cargo install builder_cpp
builder_cpp -b
```

To clean build, run

```
builder_cpp -rb
```

To run
```
builder_cpp -r
```

The binaries will be placed in the bin directory
![clk](https://user-images.githubusercontent.com/64161204/202875201-9323d65a-1955-4972-ba35-8dc294f3800a.png)
