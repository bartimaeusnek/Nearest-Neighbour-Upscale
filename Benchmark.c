#include <float.h>
#include "NearestNeighbourUpscaleDriver.h"
#include "DirectoryHandler.h"

double get_current_time();
void upscaleNN_RGBA_original(unsigned char *originalImg, unsigned char *upscaledImg, int dimX, int dimY, int scale);
#ifndef Iterations
#define Iterations 10000
#endif
int main() {
#ifdef DEBUG
    printf("You may not run the Benchmark in Debug mode!\n");
    exit(1);
#endif
    printf("Setup image array\n");

    uint8_t *img = NULL;
    uint32_t dimX = 0;
    uint32_t dimY = 0;
    char* ptr = make_absolute_path("TestImages\\test_mini.png");
    LoadImageFromDisk(ptr, &img, &dimX, &dimY);
    free(ptr);

    for (uint32_t scale = 2; scale < 16; ++scale) {
        printf("######################\nScale: %d\n", scale);
        uint32_t scaledDimX = dimX * scale;
        uint32_t scaledDimY = dimY * scale;
        uint32_t maxScaledPixel = scaledDimX * scaledDimY;

        uint32_t *upscaledImg = (uint32_t *) malloc((size_t) maxScaledPixel * (size_t) sizeof(uint32_t));

        if (NULL == upscaledImg) {
            fprintf(stderr, "Unable to allocate upscaledImg array... May have run out of RAM.\n");
            free(img);
            exit(EXIT_FAILURE);
        }
        double startTime, endTime;
        double maxTime = DBL_MIN;
        double minTime = DBL_MAX;
        double itstartTime, itendTime, benchtime;

        printf("Test original performance\n");
        for (int i = 0; i < Iterations; ++i) {
            upscaleNN_RGBA_original(img, upscaledImg, dimX, dimY, scale);
        }
        startTime = get_current_time();
        for (int i = 0; i < Iterations; ++i) {
            itstartTime = get_current_time();
            upscaleNN_RGBA_original(img, upscaledImg, dimX, dimY, scale);
            itendTime = get_current_time();
            benchtime = itendTime - itstartTime;
            if (benchtime > maxTime)
                maxTime = benchtime;
            if (minTime > benchtime)
                minTime = benchtime;
        }
        endTime = get_current_time();
        
        printf("Mean: %f\n", (endTime - startTime) / Iterations);
        printf("Max: %f\n", maxTime);
        printf("Min: %f\n", minTime);
        maxTime = DBL_MIN;
        minTime = DBL_MAX;

        printf("Test naive new impl performance\n");
        for (int i = 0; i < Iterations; ++i) {
            upscaleNN_Simple((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
        }
        startTime = get_current_time();
        for (int i = 0; i < Iterations; ++i) {
            itstartTime = get_current_time();
            upscaleNN_Simple((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            itendTime = get_current_time();
            benchtime = itendTime - itstartTime;
            if (benchtime > maxTime)
                maxTime = benchtime;
            if (minTime > benchtime)
                minTime = benchtime;
        }
        endTime = get_current_time();
        printf("Mean: %f\n", (endTime - startTime) / Iterations);
        printf("Max: %f\n", maxTime);
        printf("Min: %f\n", minTime);
        maxTime = DBL_MIN;
        minTime = DBL_MAX;

        printf("Test dynamic new impl performance\n");
        for (int i = 0; i < Iterations; ++i) {
            upscaleNN_Dynamic((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
        }
        startTime = get_current_time();
        for (int i = 0; i < Iterations; ++i) {
            itstartTime = get_current_time();
            upscaleNN_Dynamic((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            itendTime = get_current_time();
            benchtime = itendTime - itstartTime;
            if (benchtime > maxTime)
                maxTime = benchtime;
            if (minTime > benchtime)
                minTime = benchtime;
        }
        endTime = get_current_time();
        printf("Mean: %f\n", (endTime - startTime) / Iterations);
        printf("Max: %f\n", maxTime);
        printf("Min: %f\n", minTime);
        maxTime = DBL_MIN;
        minTime = DBL_MAX;

#ifdef AVX_TESTS
        if (scale %8 == 0) {
            printf("Test AVX2_1x8 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_AVX2_1x8((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_AVX2_1x8((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;

            printf("Test AVX2_8x8 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_AVX2_8x8((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_AVX2_8x8((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;
        }
#endif
#ifdef SSE_TESTS
        if (scale % 4 == 0) {
            printf("Test SSE3_4x4 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_SSE3_4x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_SSE3_4x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;

            printf("Test SSE3_2x4 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_SSE3_2x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_SSE3_2x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;

            printf("Test SSE3_1x4 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_SSE3_1x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_SSE3_1x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;
        }
        if (scale % 2 == 0) {
            printf("Test SSE3_2x2 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_SSE3_2x2((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_SSE3_2x2((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;

            printf("Test SSE3_1x2 performance\n");
            for (int i = 0; i < Iterations; ++i) {
                upscaleNN_SSE3_1x2((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
            }
            startTime = get_current_time();
            for (int i = 0; i < Iterations; ++i) {
                itstartTime = get_current_time();
                upscaleNN_SSE3_1x2((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
                itendTime = get_current_time();
                benchtime = itendTime - itstartTime;
                if (benchtime > maxTime)
                    maxTime = benchtime;
                if (minTime > benchtime)
                    minTime = benchtime;
            }
            endTime = get_current_time();
            printf("Mean: %f\n", (endTime - startTime) / Iterations);
            printf("Max: %f\n", maxTime);
            printf("Min: %f\n", minTime);
            maxTime = DBL_MIN;
            minTime = DBL_MAX;
        }
#endif
        free(upscaledImg);
    }
    free(img);
}


#define CHANNELS_PER_PIXEL_RGBA 4
void upscaleNN_RGBA_original(unsigned char *originalImg, unsigned char *upscaledImg, int dimX, int dimY, int scale){
    long long int scaledDimX = (long long int)(dimX * scale);	// X Dimension of the upscaled image.
    long long int scaledDimY = (long long int)(dimY * scale);	// Y Dimension of the upscaled image.

    // STEP 1. Place each pixel from the original image into the top-left corner of it's respective expanded pixel in the scaled up image.
#pragma omp parallel for
    for(long long int pixels = 0; pixels < dimX * dimY; pixels++){
        long long int rowsOffset = (pixels / dimX) * (long long int)(pixels != 0) * scaledDimX * (long long int)(scale - 1) * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the rows that come before where we want to write.
        long long int colsOffset = pixels * (long long int)scale * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the number of expanded pixels that come before where we want to write in this row.

        upscaledImg[colsOffset + rowsOffset] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA];										// R
        upscaledImg[colsOffset + rowsOffset + (long long int)1] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA + (long long int)1];	// G
        upscaledImg[colsOffset + rowsOffset + (long long int)2] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA + (long long int)2];	// B
        upscaledImg[colsOffset + rowsOffset + (long long int)3] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA + (long long int)3];	// A
    }

    // STEP 2. Go through all the rows containing the topmost row of each expanded pixel and fill in the blank spots with the data from the top-left pixel which we placed in step 1.
#pragma omp parallel for
    for(long long int rows = 0; rows < dimY; rows++){
        long long int rowsOffset = rows * (long long int)scale * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the rows that come before where we want to write.
        // For all the expanded pixels in the current row.
        for(long long int cols = 0; cols < dimX; cols++){
            long long int offset = rowsOffset + (cols * (long long int)scale * (long long int)CHANNELS_PER_PIXEL_RGBA);	// Account for the rowsOffset and the number of expanded pixels we have already written to in this row.

            // Copy the leftmost pixel to the other blank pixels in this row of the expanded pixel. (Starts at 1 because that is the one that was populated in step 1 and we are copying from it.)
            for(int pixelCol = 1; pixelCol < scale; pixelCol++){
                upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA)] = upscaledImg[offset];										// R
                upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA) + (long long int)1] = upscaledImg[offset + (long long int)1];	// G
                upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA) + (long long int)2] = upscaledImg[offset + (long long int)2];	// B
                upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA) + (long long int)3] = upscaledImg[offset + (long long int)3];	// A
            }
        }
    }

    // STEP 3. Complete the remaining rows by cloning the completed first row of the expanded pixels we placed in step 2 into all of the blank rows remaining of the expanded pixels.
#pragma omp parallel for
    for(long long int rows = 1; rows < scaledDimY; rows++){
        // Make sure this row is not one of the top rows of an expanded pixel.
        if((rows % (long long int)scale) != 0){
            long long int rowsOffset = rows * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the rows that come before where we want to write.
            long long int copyFromOffset = (rows - (rows % (long long int)scale)) * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Figure out which row we should copy data from. (Should be the first row in this expanded pixel.)

            // For all pixels in this row of the expanded pixel, copy the value from the first row of this expanded pixel which was filled out in STEP 2.
            for(long long int pixelCol = 0; pixelCol < scaledDimX; pixelCol++){
                long long int pixelOffset = pixelCol * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for how many pixels we have written to in this row already.

                upscaledImg[pixelOffset + rowsOffset] = upscaledImg[copyFromOffset + pixelOffset];											// R
                upscaledImg[pixelOffset + rowsOffset + (long long int)1] = upscaledImg[copyFromOffset + pixelOffset + (long long int)1];	// G
                upscaledImg[pixelOffset + rowsOffset + (long long int)2] = upscaledImg[copyFromOffset + pixelOffset + (long long int)2];	// B
                upscaledImg[pixelOffset + rowsOffset + (long long int)3] = upscaledImg[copyFromOffset + pixelOffset + (long long int)3];	// A
            }
        }
    }
}

#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <windows.h>
#include <winnt.h>
#include <stdio.h>
double get_current_time() {
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}
#else
#include <sys/time.h>
#include <sys/resource.h>
double get_current_time() {
	struct timeval t;
	gettimeofday(&t, 0);
	return t.tv_sec + t.tv_usec*1e-6;
}
#endif