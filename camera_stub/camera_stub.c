#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <math.h>
#include "camera_stub.h"

static uint8_t* g_arena = 0;    // head of ring buffer
static int g_arena_pool_index = 0;
#define POOL_NUM 10

int CameraStub_initialize(){
    if(!g_arena){
        g_arena = (uint8_t*)memalign(CAMERA_ADDR_ALIGN, CAMERA_STRIDE*CAMERA_HEIGHT*POOL_NUM);
        assert(g_arena);

        memset(g_arena, 0, CAMERA_STRIDE*CAMERA_HEIGHT*POOL_NUM);
        for(int i = 0; i < POOL_NUM; i++){
            CameraStub_drawTestImage(g_arena+CAMERA_STRIDE*CAMERA_HEIGHT*i,
                    CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_STRIDE);
        }
    }
    g_arena_pool_index = 0;

    return 0;
}

int CameraStub_finalize(){
    if(g_arena){
        free(g_arena);
        g_arena = 0;
    }
    g_arena_pool_index = 0;

    return 0;
}

int CameraStub_setParam(CameraParam_t* param){
    return 0;
}

int CameraStub_getImage(int type, uint8_t** data, int* width, int* height, int* stride) {
    assert(g_arena);

    (void)type;

    *width = CAMERA_WIDTH;
    *height = CAMERA_HEIGHT;
    *stride = CAMERA_STRIDE;

    *data = g_arena + (CAMERA_STRIDE*CAMERA_HEIGHT)*g_arena_pool_index;
    g_arena_pool_index++;
    if(POOL_NUM <= g_arena_pool_index){
        g_arena_pool_index = 0;
    }

    return 0;
}

int CameraStub_releaseImage(uint8_t* data) {
    return 0;
}

void CameraStub_drawTestImage(uint8_t* data, const int width, const int height, const int stride){
    // load Image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t v = 0;
            if (0.5f * x + 10.0f < y) {
                v = 128;
            }
            data[stride*y + x] = v;
        }
    }

    // add random noise
    for(int i = 0; i < height/5; i++){
        int x = rand() % width;
        int y = rand() % height;
        data[stride*y + x] = 128;
    }
}

