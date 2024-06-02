#include "NearestNeighbourUpscaleDriver.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include "unistd.h"
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

bool loadImgRGBA(uint8_t** img, uint32_t* width, uint32_t* height, char* filename){
    uint8_t *png = NULL;
	size_t pngsize;
	LodePNGState state;

	lodepng_state_init(&state);

	unsigned error = lodepng_load_file(&png, &pngsize, filename);
	if(!error){
		error = lodepng_decode(img, width, height, &state, png, pngsize);
		if(error){
			printf("Lodepng encountered an error.\nError: %u, %s\n", error, lodepng_error_text(error));
            exit(EXIT_FAILURE);
		}
		free(png);
        size_t channels = lodepng_get_channels(&state.info_raw);
		lodepng_state_cleanup(&state);
		return channels == 4;
	}
	else{
		printf("Lodepng encountered an error.\nError: %u, %s\n", error, lodepng_error_text(error));
        exit(EXIT_FAILURE);
	}
}


void LoadImageFromDisk(char *filename, uint8_t **img, uint32_t *dimX,
                         uint32_t *dimY) {
    bool loaded = loadImgRGBA(img, dimX, dimY, filename);
    if(!loaded) {
        if (*img != NULL)
            free((*img));
        exit(EXIT_FAILURE);
    }
}

void UpscaleImage(char *filename, uint32_t scale) {
    uint8_t *img = NULL;
    uint32_t dimX = 0;
    uint32_t dimY = 0;
    LoadImageFromDisk(filename, &img, &dimX, &dimY);

    uint32_t scaledDimX = dimX * scale;
    uint32_t scaledDimY = dimY * scale;
    uint32_t maxScaledPixel = scaledDimX * scaledDimY;

    uint32_t *upscaledImg = (uint32_t*)malloc((size_t)maxScaledPixel * (size_t)sizeof(uint32_t));

    if(NULL == upscaledImg) {
        fprintf(stderr, "Unable to allocate upscaledImg array... May have run out of RAM.\n");
        free(img);
        exit(EXIT_FAILURE);
    }

#if __AVX__
    if (scale % 8 == 0 && dimX % 8 == 0) {
        upscaleNN_AVX2_8x8((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else if (scale % 8 == 0) {
        upscaleNN_AVX2_1x8((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else
#endif
#if __SSE3__
    if (scale % 4 == 0 && dimX % 4 == 0) {
        upscaleNN_SSE3_4x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else if (scale % 4 == 0 && dimX % 2 == 0) {
        upscaleNN_SSE3_2x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else if (scale % 4 == 0) {
        upscaleNN_SSE3_1x4((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else if (scale % 2 == 0 && dimX % 2 == 0) {
        upscaleNN_SSE3_2x2((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else if (scale % 2 == 0) {
        upscaleNN_SSE3_1x2((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    else
#endif
    {
        if (scale <= 3)
            upscaleNN_Simple((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
        else
            upscaleNN_Dynamic((const uint32_t *) img, upscaledImg, dimX, dimY, scale);
    }
    free(img);
#ifndef DEBUG
    lodepng_encode32_file(filename, (const unsigned char *) upscaledImg, (size_t)dimX * scale, (size_t)dimY * scale);
#endif
    free(upscaledImg);
}