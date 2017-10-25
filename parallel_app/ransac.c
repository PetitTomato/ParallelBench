#include "ransac.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct point {
    float x;
    float y;
} point_t;

static model_t getParamWithSamples(point_t p, point_t q) {
    float dx = q.x - p.x;
    float dy = q.y - p.y;
    model_t m;
    if(fabs(dy) < 0.00001f)
        m.a = 100000.0f;
    else
        m.a = dy / dx;
    m.b = p.y - p.x * m.a;
    m.error = 0.0f;

    return m;
}

static float applyModel(model_t m, float x) {
    return m.a * x + m.b;
}

static float getError(model_t m, point_t p) {
    return fabs(applyModel(m, p.x) - p.y);
}

int ransac(const float* data, const int sample_num, model_t* model, const int iteration_num) {
    model->a = 1.0f;
    model->b = 1.0f;

    model_t candidate;
    memset(&candidate, 0, sizeof(model_t));
    candidate.error = FLT_MAX;

    int inliers = 0;
    int isValid = 0;

    for (int i = 0; i < iteration_num; i++) {
        const int r1 = rand() % sample_num;
        const int r2 = rand() % sample_num;

        point_t p1;
        p1.x = data[r1 * 2];
        p1.y = data[r1 * 2 + 1];
        point_t p2;
        p2.x = data[r2 * 2];
        p2.y = data[r2 * 2 + 1];

        model_t m = getParamWithSamples(p1, p2);

        inliers = 0;
        m.error = 0.0f;
        for(int j = 0; j < sample_num; j++){
            if(j == r1 || j == r2)
                continue;

            point_t p;
            p.x = data[j*2];
            p.y = data[j*2+1];
            float error = getError(m, p);
            m.error += error;
            if(error < 2.0f){
                inliers++;
            }
        }
        m.error /= sample_num;  // get mean

        if(sample_num*0.8f < inliers){
            if(m.error < candidate.error){
                candidate = m;
                isValid = 1;
            }
        }
    }

    *model = candidate;

    return isValid ? 0 : 1;
}

void ransac_test(){
    model_t m;
    m.a = 0.1f;
    m.b = -0.5f;

    int sample_num = 1000;
    float* data = (float*)malloc(sizeof(float)*sample_num*2);
    for(int i = 0; i < sample_num; i++){
        float x = (rand()/(float)RAND_MAX - 0.5f) * 100.0f;
        float y = applyModel(m, x);
        float noise = (rand()/(float)RAND_MAX - 0.5f) * 2.0f;
        data[i*2] = x;
        data[i*2 + 1] = y + noise;
    }

    model_t candidate;
    int err = ransac(data, sample_num, &candidate, 100);
    printf("err: %d, %lf %lf : %lf %lf\n", err, m.a, m.b, candidate.a, candidate.b);
    free(data);
}
