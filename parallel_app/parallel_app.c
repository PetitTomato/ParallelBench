#include "parallel_app.h"

#include <string.h>
#include <assert.h>
#include "ransac.h"

void parallel_app_init(context_t* ctx) {
    memset(ctx, 0, sizeof(context_t));
    ctx->state = A;
}

void parallel_app_fin(context_t* ctx) {

}

int parallel_app(context_t* ctx, image_t *img, result_t *result) {
    // pre process
    getBlurImage(img, &ctx->blurImg);

    // create pyramid
    {
        ctx->pyramidImg[0] = ctx->blurImg;
        for (int i = 0; i < LEVEL - 1; i++) {
            image_t* src = &ctx->pyramidImg[i];
            image_t* dst = &ctx->pyramidImg[i + 1];
            getDownsampledImage(src, dst);
        }
    }

    // main process
    if (ctx->state == A) {
        getFeaturePoints(&ctx->pyramidImg[0], &ctx->featureArray[0]);
        //printf("fp: %d\n", ctx->featureArray[i].size);
        int err = ransac(ctx->featureArray[0].pos, ctx->featureArray[0].size, &ctx->model[0], 1000);
        //printf("err: %d,  %lf %lf\n", err, ctx->model[0].a, ctx->model[0].b);
        if(!err){
            ctx->state = B;
        }
    } else if (ctx->state == B) {
        for (int i = 0; i < LEVEL; i++) {
            getFeaturePoints(&ctx->pyramidImg[i], &ctx->featureArray[i]);
            //printf("fp: %d\n", ctx->featureArray[i].size);
            int err = ransac(ctx->featureArray[i].pos, ctx->featureArray[i].size, &ctx->model[i], 100 + (i * 100));

            if(i == 0){
                ctx->tmp1 = err ? 0 : 1;
            } else if(i == 1){
                ctx->tmp2 = err ? 0 : 1;
            } else if(i == 2){
                ctx->tmp3 = err ? 0 : 1;
            }
        }

        ctx->state = C;
    } else if (ctx->state == C) {
        ctx->state = A;
    } else {
        assert(0 && "illigal state");
    }

    // post process
    result->n = ctx->tmp1 + ctx->tmp2 + ctx->tmp2;

    // finalize
    for (int i = 0; i < LEVEL; i++) {
        releaseDownsampledImage(&ctx->pyramidImg[i]);  // i==0 is blur img.
        releaseFeaturePoints(&ctx->featureArray[i]);
    }

    return 0;
}
