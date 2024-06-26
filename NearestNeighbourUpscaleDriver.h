#ifndef NEARESTNEIGHBOURUPSCALEDRIVER_H
#define NEARESTNEIGHBOURUPSCALEDRIVER_H

#include "MimallocOverrideCOnly.h"
#include "lodepng/lodepng.h"
#include "NearestNeighbourUpscale.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include "unistd.h"
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

bool loadImgRGBA(uint8_t** img, uint32_t* width, uint32_t* height, char* filename);
void LoadImageFromDisk(char *filename, uint8_t **img, uint32_t *dimX, uint32_t *dimY);
void UpscaleImage(char *filename, uint32_t scale);

#endif
