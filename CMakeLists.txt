cmake_minimum_required(VERSION 3.25)
project(NearestNeighbourUpscale C)
set(CMAKE_C_STANDARD 99)

option(SSE3_BUILD "Build with SSE3 support" ON)
option(AVX_BUILD "Build with AVX support" ON)
option(NATIVE_BUILD "Build for Native CPU" OFF)
option(AVX_TESTS "Add specific AVX Tests" OFF)
option(SSE_TESTS "Add specific SSE Tests" OFF)

if (AVX_TESTS)
    add_compile_definitions(AVX_TESTS=1)
endif ()
if (SSE_TESTS)
    add_compile_definitions(SSE_TESTS=1)
endif ()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(OpenMP_C "${CMAKE_C_COMPILER}")
    set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
    set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
    set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
    set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
    set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
endif()
if (AVX_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")
endif ()
if (SSE3_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse3")
endif ()
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG=1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g0")
endif()
if(NATIVE_BUILD)
    CHECK_C_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
    endif()
endif()

add_executable(NearestNeighbourUpscale
        NearestNeighbourUpscale.c
        NearestNeighbourUpscale.h
        NearestNeighbourUpscaleDriver.c
        NearestNeighbourUpscaleDriver.h
        unistd.h
        dirent.h
        DirectoryHandler.c
        DirectoryHandler.h
        MimallocOverrideCOnly.h
        NNUMain.c
)

add_compile_definitions(LODEPNG_NO_COMPILE_ALLOCATORS=1)
set_source_files_properties(lodepng/lodepng.cpp PROPERTIES LANGUAGE C)
add_library(lodepng STATIC lodepng/lodepng.cpp lodepng/lodepng.h lodepng_mimalloc_adapter.c)
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
add_subdirectory(mimalloc)
target_link_libraries(lodepng PUBLIC mimalloc-static)
target_link_libraries(NearestNeighbourUpscale PUBLIC mimalloc-static lodepng)
message("CMakeFlags: ${CMAKE_C_FLAGS}")

add_executable(Benchmark
        Benchmark.c
        NearestNeighbourUpscale.c
        NearestNeighbourUpscale.h
        NearestNeighbourUpscaleDriver.c
        NearestNeighbourUpscaleDriver.h
        unistd.h
        dirent.h
        DirectoryHandler.c
        DirectoryHandler.h
        MimallocOverrideCOnly.h
)
target_link_libraries(Benchmark PUBLIC mimalloc-static lodepng)