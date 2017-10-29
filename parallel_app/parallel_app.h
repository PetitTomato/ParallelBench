#ifndef PARALLEL_APP_H_
#define PARALLEL_APP_H_

#include <stdint.h>
#include "image.h"
#include "ransac.h"

typedef struct param {
    int n;
} param_t;

typedef struct result {
    int n;
} result_t;

typedef enum state {
    A, B, C, state_max
} state_t;

#define LEVEL 5
typedef struct context {
    state_t state;
    image_t blurImg;
    image_t pyramidImg[LEVEL];
    model_t model[LEVEL];
    float featureThresh[LEVEL];
    featurePoints_t featureArray[LEVEL];
    int tmp1;
    int tmp2;
    int tmp3;
} context_t;

int parallel_app(context_t* ctx, image_t *img, result_t *n);

void parallel_app_init(context_t* ctx);
void parallel_app_fin(context_t* ctx);

#endif /* PARALLEL_APP_H_ */
