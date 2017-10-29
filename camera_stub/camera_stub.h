#ifndef CAMERA_STUB_H_
#define CAMERA_STUB_H_

#include <stdint.h>

#if 0
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define CAMERA_STRIDE 640
#else
#define CAMERA_WIDTH 1920
#define CAMERA_HEIGHT 1080
#define CAMERA_STRIDE 1920
#endif

#define CAMERA_ADDR_ALIGN   64
#define CAMERA_STRIDE_ALIGN 16

typedef struct CameraParam {
    uint8_t a;
    uint16_t b;
    uint8_t c;
    uint32_t d;
    uint8_t e;
} CameraParam_t;

int CameraStub_initialize();
int CameraStub_finalize();

int CameraStub_setParam(CameraParam_t* param);
int CameraStub_getImage(int type, uint8_t** data, int* width, int* height, int* stride);
int CameraStub_releaseImage(uint8_t* data);

void CameraStub_drawTestImage(uint8_t* data, const int width, const int height, const int stride);

#endif /* CAMERA_STUB_H_ */
