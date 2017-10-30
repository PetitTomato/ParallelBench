#include "image.h"
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "../camera_stub/camera_stub.h"
#define USE_CAMERA 0


void imageSystemInitialize(){
#if USE_CAMERA
    CameraStub_initialize();
#endif
}

void imageSystemFinalize(){
#if USE_CAMERA
    CameraStub_finalize();
#endif
}

static void image_init(image_t* n) {
    memset(n, 0, sizeof(image_t));
}

static int releaseImage(image_t* n) {
    if (n->data) {
        free(n->data);
        n->data = 0;
    }

    return 0;
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(v, min, max) (MIN(MAX((min), (v)), (max)))

static inline uint8_t getValue(image_t* src, int x, int y) {
	int cx = CLAMP(x, 0, src->width-1);
	int cy = CLAMP(y, 0, src->height-1);
    return src->data[cy * src->stride + cx];
}
static inline void setValue(image_t* src, int x, int y, uint8_t v) {
	int cx = CLAMP(x, 0, src->width-1);
	int cy = CLAMP(y, 0, src->height-1);
    src->data[cy * src->stride + cx] = v;
}

int getCameraImage(int i, image_t* n) {
    image_init(n);
#if USE_CAMERA
    CameraStub_getImage(0, &n->data, &n->width, &n->height, &n->stride);
#else
    void* p = malloc(CAMERA_STRIDE * CAMERA_HEIGHT);
    if (p) {
        n->data = p;
        n->width = CAMERA_WIDTH;
        n->height = CAMERA_HEIGHT;
        n->stride = CAMERA_STRIDE;
        CameraStub_drawTestImage(n->data, n->width, n->height, n->stride);
    }
#endif

    n->timestamp = i * 1000;

    return 0;
}

int releaseCameraImage(image_t* n){
#if USE_CAMERA
    CameraStub_releaseImage(n->data);
#else
    releaseImage(n);
#endif
    return 0;
}

int getBlurImage(image_t* src, image_t* dst) {
    image_init(dst);
    void* p = malloc(src->stride * src->height);
    if (p) {
        *dst = *src;	// copy w, h, stride
        dst->data = p;

        // make blur image
        for(int y = 0; y < dst->height; y++){
        	for(int x = 0; x < dst->width; x++){
        		int v = 0;
				v += 1*getValue(src, x-1, y-1);
				v += 2*getValue(src, x  , y-1);
				v += 1*getValue(src, x+1, y-1);
				v += 2*getValue(src, x-1, y);
				v += 4*getValue(src, x  , y);
				v += 2*getValue(src, x+1, y);
				v += 1*getValue(src, x-1, y+1);
				v += 2*getValue(src, x  , y+1);
				v += 1*getValue(src, x+1, y+1);
        		setValue(dst, x, y, v/16);
        	}
        }
    }
    dst->timestamp = src->timestamp;

    return 0;
}

int releaseBlurImage(image_t *n){
    releaseImage(n);
    return 0;
}

int getDownsampledImage(image_t* src, image_t* dst) {
    image_init(dst);
    dst->width = src->width / 2;
    dst->stride = dst->width;
    dst->height = src->height / 2;
    void* p = malloc(dst->stride * dst->height);
    if (p) {
        dst->data = p;
        // make downsampled image
        for (int y = 0; y < dst->height; y++) {
            for (int x = 0; x < dst->width; x++) {
                setValue(dst, x, y, getValue(src, x * 2, y * 2));
            }
        }
    }
    dst->timestamp = src->timestamp;

    return 0;
}

int releaseDownsampledImage(image_t *n){
    releaseImage(n);
    return 0;
}


void featurePoints_init(featurePoints_t* n) {
    memset(n, 0, sizeof(featurePoints_t));
}

static inline int featurePoints_add(featurePoints_t* dst, float x, float y) {
    if (dst->size < dst->capacity) {
        dst->pos[dst->size * 2] = x;
        dst->pos[dst->size * 2 + 1] = y;
        dst->size++;
        return 0;
    } else {
        return 1;	// err
    }
}

int getFeaturePoints(image_t* src, featurePoints_t* featurePoints, float* th) {
    featurePoints_init(featurePoints);

    featurePoints->pos = malloc(sizeof(float) * 2 * 1000);
    featurePoints->capacity = 1000;
    featurePoints->size = 0;
    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width - 1; x++) {
            float score = fabs(getValue(src, x, y) - getValue(src, x + 1, y));
            if (*th < score) {
                featurePoints_add(featurePoints, x, y);
            }
        }
    }

    float new_th = *th;
    if(featurePoints->capacity == featurePoints->size){
        new_th += 5.0f;
    } else if(featurePoints->size < featurePoints->capacity/2){
        new_th -= 1.0f;
    }
    CLAMP(new_th, 10.0f, 150.0f);
    *th = new_th;

    return 0;
}

int releaseFeaturePoints(featurePoints_t* featurePoints) {
    if (featurePoints->pos) {
        free(featurePoints->pos);
        featurePoints->pos = 0;
    }

    return 0;
}

