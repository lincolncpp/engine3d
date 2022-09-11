#ifndef ENGINE3D_UTILS
#define ENGINE3D_UTILS

struct Point{
    float x, y, z;
    Point(){}
    Point(float x, float y, float z) : x(x), y(y), z(z){}
};

void matrix_mul(float *a, float *b, float *c);

#endif