#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>

typedef struct image {
    uint8_t *data;
    uint64_t timestamp;
    uint16_t width;
    uint16_t height;
    uint16_t stride;
} image_t;

void imageSystemInitialize();
void imageSystemFinalize();

// users doesn't have to allocate memory.
int getCameraImage(int frameNo, image_t *n);
int releaseCameraImage(image_t* n);
int getBlurImage(image_t* src, image_t* dst);
int releaseBlurImage(image_t *n);
int getDownsampledImage(image_t* src, image_t* dst);
int releaseDownsampledImage(image_t *n);

typedef struct featurePoints {
    float *pos;	// x, y, x, y, ...
    uint16_t capacity;
    uint16_t size;
} featurePoints_t;

int getFeaturePoints(image_t* src, featurePoints_t* featurePoints);
int releaseFeaturePoints(featurePoints_t* featurePoints);

#endif /* IMAGE_H_ */
