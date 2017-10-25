#include <malloc.h>
#include <assert.h>
#include "camera_stub.h"

static uint8_t* g_arena = 0;    // head of ring buffer
static int g_arena_pool_index = 0;
#define POOL_NUM 10

int CameraStub_initialize(){
    if(!g_arena){
        g_arena = (uint8_t*)malloc(CAMERA_STRIDE*CAMERA_HEIGHT*POOL_NUM);
        assert(g_arena);

        memset(g_arena, 0, CAMERA_STRIDE*CAMERA_HEIGHT*POOL_NUM);
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
    if (data) {
        free((void*) data);
        *data = 0;
    }
    return 0;
}
