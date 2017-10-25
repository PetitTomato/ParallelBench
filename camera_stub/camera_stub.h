#ifndef CAMERA_STUB_H_
#define CAMERA_STUB_H_

#include <stdint.h>

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define CAMERA_STRIDE 640

int CameraStub_initialize();
int CameraStub_finalize();

int CameraStub_getImage(int type, uint8_t** data, int* width, int* height, int* stride);
int CameraStub_releaseImage(uint8_t* data);

#endif /* CAMERA_STUB_H_ */
