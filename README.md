# Nearest-Neighbor-Upscale
C program to quickly perform nearest neighbour upscaling of an image.  
Driver code works on 32bit PNG images. Upscaler functions should work with any image format if you can extract the values of each pixel in the image into an array.  
Should work anywhere POSIX complient really...  

Pictures are supported roughly up to 32768x32768 pixels.  

## Downloads

Prebuild downloads are available on the [releases page for Windows and Linux](https://github.com/bartimaeusnek/Nearest-Neighbour-Upscale/releases) or cutting edge builds are under [Actions](https://github.com/bartimaeusnek/Nearest-Neighbour-Upscale/actions). 

## How to build on Windows:  

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
  
## How to build on Linux x64:  
You might need to install git, clang(preferred) or gcc, libomp5, libomp-dev and cmake with your package manager:  
for Ubuntu/Debian/Linux Mint and alike: `sudo apt install git clang cmake libomp5 libomp-dev`

- `git clone --recurse-submodules https://github.com/bartimaeusnek/Nearest-Neighbour-Upscale`
- `cd Nearest-Neighbour-Upscale`
- `cmake -DCMAKE_C_COMPILER=clang -S . -B build`
- `cmake --build ./build --config Release`

Your compiled binary file will be at `<path-to-Nearest-Neighbour-Upscale>/build/NearestNeighbourUpscale`

## How to build for non-x64 (i.E. arm):
- `git clone --recurse-submodules https://github.com/bartimaeusnek/Nearest-Neighbour-Upscale`
- `cd Nearest-Neighbour-Upscale`
- `cmake -DSSE3_BUILD=off -DAVX_BUILD=OFF -DCMAKE_C_COMPILER=clang -S . -B build`
- `cmake --build ./build --config Release`

Your compiled binary file will be at `<path-to-Nearest-Neighbour-Upscale>/build/NearestNeighbourUpscale`

## How to use:
- `./NearestNeighbourUpscale <INPUT_IMAGE/DIRECTORY> <SCALE>`
  - Where `<INPUT_IMAGE/DIRECTORY>` is the image or directory you want to upscale (local and absolute paths are supported) and `<SCALE>` (optional, default 2) the scale you want to upscale it by.

## Troubleshooting:

For Windows you will need to append `-G "Ninja Multi-Config" -D CMAKE_C_COMPILER="<path-to-clang>" -D CMAKE_MAKE_PROGRAM="<path-to-ninja>"` to all of these.

If you have a Processor older than 2011 please use:
- `cmake -DAVX_BUILD=OFF -DCMAKE_C_COMPILER=clang -S . -B build`

If you have an insanely old Processor please follow the non-x64 build:
- `cmake -DSSE3_BUILD=off -DAVX_BUILD=OFF -DCMAKE_C_COMPILER=clang -S . -B build`

If you are building only for yourself and don't want compability with any processor other than the one you are using to compile this, you might want to turn on:
- `cmake -DNATIVE_BUILD=ON -DCMAKE_C_COMPILER=clang -S . -B build`

If you want to disable [mimalloc](https://github.com/microsoft/mimalloc), because you are having issues with seemingly random exit codes AND you are running on a non-x64 System or very old System, or want the standard C allocator for some reason:
- `cmake -DMIMALLOC_LINKED_BUILD=OFF -DCMAKE_C_COMPILER=clang -S . -B build`

Help! I have generated a .sln file and it does not work! FIX PLEASE!

- The MSVC compiler IS NOT SUPPORTED. [Use Clang instead!](https://learn.microsoft.com/en-us/cpp/build/clang-support-msbuild?view=msvc-170)
