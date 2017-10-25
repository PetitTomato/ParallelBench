#include <stdint.h>
#include <stdio.h>
#include "image.h"
#include "parallel_app.h"

int main(void) {
    // test application

    // initialize lib
    imageSystemInitialize();
    context_t ctx;
    parallel_app_init(&ctx);
    result_t result;
    result.n = 0;

    // per frame process
    image_t img;
    for (int i = 0; i < 100; i++) {
        int err = getCameraImage(i, &img);
        if (err) {
            exit(1);
        }

        parallel_app(&ctx, &img, &result);

        releaseCameraImage(&img);

        printf("r: %d\n", result.n);
    }

    // finalize lib
    parallel_app_fin(&ctx);
    imageSystemFinalize();

    return 0;
}
