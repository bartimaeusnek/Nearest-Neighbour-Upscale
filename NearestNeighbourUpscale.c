#include "NearestNeighbourUpscale.h"

void upscaleNN_Simple(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    const size_t scaledDimX = dimX * scale;
#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        const size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; ++x) {
            const uint32_t pixel = originalImg[y * dimX + x];
            const size_t upscaledColStart = x * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; ++dx) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    upscaledImg[offset] = pixel;
                }
            }
        }
    }
}

#if __SSE3__
void upscaleNN_SSE3_1x2(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; ++x) {
            __m128i pxt = _mm_lddqu_si128((const __m128i *) (originalImg + (y * dimX + x)));
            _mm_moveldup_ps((__m128) pxt);
            size_t upscaledColStart = x * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 2) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    _mm_storeu_si64(upscaledImg + offset, pxt);
                }
            }
        }
    }
}
void upscaleNN_SSE3_2x2(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; x += 2) {
            __m128i pxt = _mm_lddqu_si128((const __m128i *) (originalImg + (y * dimX + x)));
            __m128i px0 = (__m128i) __builtin_shufflevector((__v4sf)pxt, (__v4sf)pxt, 0, 0, 0, 0);
            __m128i px1 = (__m128i) __builtin_shufflevector((__v4sf)pxt, (__v4sf)pxt, 1, 1, 1, 1);
            size_t upscaledCol0Start = x * scale;
            size_t upscaledCol1Start = (x + 1) * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 2) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol0Start + dx);
                    _mm_storeu_si64(upscaledImg + offset, px0);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol1Start + dx);
                    _mm_storeu_si64(upscaledImg + offset, px1);
                }
            }
        }
    }
}
void upscaleNN_SSE3_4x4(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; x += 4) {
            __m128i px = _mm_lddqu_si128((const __m128i *) (originalImg + (y * dimX + x)));
            __m128i px0 = (__m128i) __builtin_shufflevector((__v4sf)px, (__v4sf)px, 0, 0, 0, 0);
            __m128i px1 = (__m128i) __builtin_shufflevector((__v4sf)px, (__v4sf)px, 1, 1, 1, 1);
            __m128i px2 = (__m128i) __builtin_shufflevector((__v4sf)px, (__v4sf)px, 2, 2, 2, 2);
            __m128i px3 = (__m128i) __builtin_shufflevector((__v4sf)px, (__v4sf)px, 3, 3, 3, 3);

            size_t upscaledCol0Start = (x + 0) * scale;
            size_t upscaledCol1Start = (x + 1) * scale;
            size_t upscaledCol2Start = (x + 2) * scale;
            size_t upscaledCol3Start = (x + 3) * scale;

            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 4) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol0Start + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px0);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol1Start + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px1);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol2Start + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px2);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol3Start + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px3);
                }
            }
        }
    }
}

void upscaleNN_SSE3_1x4(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; ++x) {
            __m128i px1 = _mm_lddqu_si128((const __m128i *) (originalImg + (y * dimX + x)));
            px1 = (__m128i) __builtin_shufflevector((__v4sf)px1, (__v4sf)px1, 0, 0, 0, 0);
            size_t upscaledColStart = x * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 4) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px1);
                }
            }
        }
    }
}
void upscaleNN_SSE3_2x4(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; x += 2) {
            __m128i pxt = _mm_lddqu_si128((const __m128i *) (originalImg + (y * dimX + x)));
            __m128i px0 = (__m128i) __builtin_shufflevector((__v4sf) pxt, (__v4sf) pxt, 0, 0, 0, 0);
            __m128i px1 = (__m128i) __builtin_shufflevector((__v4sf) pxt, (__v4sf) pxt, 1, 1, 1, 1);
            size_t upscaledCol0Start = x * scale;
            size_t upscaledCol1Start = (x + 1) * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 4) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol0Start + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px0);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol1Start + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), px1);
                }
            }
        }
    }
}
#endif
#if __AVX__
void upscaleNN_AVX2_8x8(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; x += 8) {
            __m256i px = _mm256_lddqu_si256((const __m256i *) (originalImg + (y * dimX + x)));
            __m256i px0 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 0, 0, 0, 0, 0, 0, 0, 0);
            __m256i px1 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 1, 1, 1, 1, 1, 1, 1, 1);
            __m256i px2 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 2, 2, 2, 2, 2, 2, 2, 2);
            __m256i px3 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 3, 3, 3, 3, 3, 3, 3, 3);
            __m256i px4 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 4, 4, 4, 4, 4, 4, 4, 4);
            __m256i px5 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 5, 5, 5, 5, 5, 5, 5, 5);
            __m256i px6 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 6, 6, 6, 6, 6, 6, 6, 6);
            __m256i px7 = (__m256i) __builtin_shufflevector((__v8sf)px, (__v8sf)px, 7, 7, 7, 7, 7, 7, 7, 7);

            size_t upscaledCol0Start = (x + 0) * scale;
            size_t upscaledCol1Start = (x + 1) * scale;
            size_t upscaledCol2Start = (x + 2) * scale;
            size_t upscaledCol3Start = (x + 3) * scale;
            size_t upscaledCol4Start = (x + 4) * scale;
            size_t upscaledCol5Start = (x + 5) * scale;
            size_t upscaledCol6Start = (x + 6) * scale;
            size_t upscaledCol7Start = (x + 7) * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 8) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol0Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px0);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol1Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px1);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol2Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px2);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol3Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px3);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol4Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px4);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol5Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px5);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol6Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px6);
                    offset = (upscaledRowStart + dy) * scaledDimX + (upscaledCol7Start + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px7);
                }
            }
        }
    }
}
void upscaleNN_AVX2_1x8(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    size_t scaledDimX = ((size_t)dimX * (size_t)scale);

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; ++x) {
            __m256i px1 = _mm256_lddqu_si256((const __m256i *) (originalImg + (y * dimX + x)));
            px1 = (__m256i) __builtin_shufflevector((__v8sf)px1, (__v8sf)px1, 0, 0, 0, 0, 0, 0, 0, 0);
            size_t upscaledColStart = x * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                for (size_t dx = 0; dx < scale; dx += 8) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), px1);
                }
            }
        }
    }
}
#endif
void upscaleNN_Dynamic(const uint32_t *originalImg, uint32_t *upscaledImg, uint32_t dimX, uint32_t dimY, uint32_t scale) {
    const size_t scaledDimX = dimX * scale;

#if __AVX__
    bool _1x8Passes = scale / 8;
#endif
#if __SSE3__
    bool _1x4Passes = (scale
#if __AVX__
            - _1x8Passes*8
#endif
            ) / 4;
    bool _1x2Passes = (scale
#if __AVX__
            - _1x8Passes*8
#endif
            - _1x4Passes*4) / 2;
#endif

#pragma omp parallel for
    for (size_t y = 0; y < dimY; ++y) {
        const size_t upscaledRowStart = y * scale;
        for (size_t x = 0; x < dimX; ++x) {
#if __AVX__
            __m256i p_1x8;
            if (_1x8Passes)
            {
                p_1x8 = _mm256_lddqu_si256((const __m256i *) (originalImg + (y * dimX + x)));
                p_1x8 = (__m256i) __builtin_shufflevector((__v8sf) p_1x8, (__v8sf) p_1x8, 0, 0, 0, 0, 0, 0, 0, 0);
            }
#endif
#if __SSE3__
            __m128i p_1x4;
            if (_1x4Passes || _1x2Passes)
            {
                p_1x4 = _mm_lddqu_si128((const __m128i *) (originalImg + (y * dimX + x)));
                p_1x4 = (__m128i) __builtin_shufflevector((__v4sf)p_1x4, (__v4sf)p_1x4, 0, 0, 0, 0);
            }
#endif
            const uint32_t pixel = originalImg[y * dimX + x];
            const size_t upscaledColStart = x * scale;
            for (size_t dy = 0; dy < scale; ++dy) {
                int dx = 0;
#if __AVX__
                for (; dx < scale /8*8; dx += 8) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    _mm256_storeu_si256((__m256i *) (upscaledImg + offset), p_1x8);
                }
#endif
#if __SSE3__
                for (; dx < scale /4*4; dx += 4) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    _mm_storeu_si128((__m128i *) (upscaledImg + offset), p_1x4);
                }
                for (; dx < scale /2*2; dx += 2) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    _mm_storeu_si64(upscaledImg + offset, p_1x4);
                }
#endif
                for (; dx < scale; ++dx) {
                    size_t offset = (upscaledRowStart + dy) * scaledDimX + (upscaledColStart + dx);
                    upscaledImg[offset] = pixel;
                }
            }
        }
    }
}