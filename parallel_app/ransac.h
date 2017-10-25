#ifndef RANSAC_H_
#define RANSAC_H_

typedef struct model {
    float a;
    float b;
    float error;
} model_t;

int ransac(const float* data, const int sample_num, model_t* model, const int iteration);

void ransac_test();

#endif /* RANSAC_H_ */
