# Nearest-Neighbor-Upscale
C program to quickly perform nearest neighbour upscaling of an image.
Driver code works on 32bit PNG images. Upscaler functions should work with any image format if you can extract the values of each pixel in the image into an array.

Pictures are supported roughly up to 32768x32768 pixels.

Prebuild downloads are available on the releases page for Windows and Linux, should work anywhere POSIX complient really...

How to build on Windows:  

With CLI:  
- [install git](https://www.git-scm.com/download/win)  
- [download Ninja](https://github.com/ninja-build/ninja/releases)  
- [install cmake](https://cmake.org/download/)  
- [install clang](https://releases.llvm.org/)
- `git clone --recurse-submodules https://github.com/bartimaeusnek/Nearest-Neighbour-Upscale`
- `cd Nearest-Neighbour-Upscale`
- `cmake -S . -B build -G "Ninja Multi-Config" -D CMAKE_C_COMPILER="<path-to-clang>" -D CMAKE_MAKE_PROGRAM="<path-to-ninja>"` (`<path-to-clang>` is usually `C:\Program Files\LLVM\bin`)
- `cmake --build ./build --config Release`

Your compiled .exe will be at `<path-to-Nearest-Neighbour-Upscale>\build\Release\NearestNeighbourUpscale.exe`  
Alternatively you can build this quite easy with [JetBrains CLion](https://www.jetbrains.com/clion/) (Not Sponsored)  
  
  
How to build on Linux:  
You might need to install git/clang/gcc and cmake with your package manager:  
for Ubuntu/Debian/Linux Mint and alike: `sudo apt install git gcc cmake`

- `git clone --recurse-submodules https://github.com/bartimaeusnek/Nearest-Neighbour-Upscale`
- `cd Nearest-Neighbour-Upscale`
- `cmake -S . -B build`
- `cmake --build ./build --config Release`

Your compiled binary file will be at `<path-to-Nearest-Neighbour-Upscale>/build/NearestNeighbourUpscale`

How to use:
- `./NearestNeighbourUpscale <INPUT_IMAGE/DIRECTORY> <SCALE>`
  - Where `<INPUT_IMAGE/DIRECTORY>` is the image or directory you want to upscale (local and absolute paths are supported) and `<SCALE>` (optional, default 2) the the scale you want to upscale it by.

Troubleshooting:

In the CMakeLists.txt:

If you are building for a non-x64 (M1 Macs i.E.) or some even older processor, please turn off
- `SSE3_BUILD`
- `AVX_BUILD`

If you have a Processor older than 2011 please turn off:
- `AVX_BUILD`

If you are building only for yourself and don't want compability with any processor other than the one you are using to compile this, you might want to turn on:
- `NATIVE_BUILD`

If you having issues with seemingly random exit codes AND you are running on a non-x64 System or very old System, you can try deleting:
- `add_compile_definitions(LODEPNG_NO_COMPILE_ALLOCATORS=1)`
- `-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free`
- `target_link_libraries(lodepng PUBLIC mimalloc-static)`
- and replace `target_link_libraries(NearestNeighbourUpscale PUBLIC mimalloc-static lodepng)` with `target_link_libraries(NearestNeighbourUpscale PUBLIC lodepng)`
- This will disable [mimalloc](https://github.com/microsoft/mimalloc) for the main library and fallback to the default allocator instead
