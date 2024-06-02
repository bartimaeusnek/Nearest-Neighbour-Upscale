#ifndef NEARESTNEIGHBOURUPSCALE_H
#define NEARESTNEIGHBOURUPSCALE_H

#include "MimallocOverrideCOnly.h"
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void upscaleNN_Simple(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
void upscaleNN_Dynamic(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
#if __AVX__
void upscaleNN_AVX2_8x8(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
void upscaleNN_AVX2_1x8(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
#endif
#if __SSE3__
void upscaleNN_SSE3_1x2(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
void upscaleNN_SSE3_2x2(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
void upscaleNN_SSE3_1x4(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
void upscaleNN_SSE3_2x4(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
void upscaleNN_SSE3_4x4(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale);
#endif
#endif
