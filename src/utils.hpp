#ifndef ENGINE3D_UTILS
#define ENGINE3D_UTILS

struct Point{
    float x, y, z;
    Point(){}
    Point(float x, float y, float z) : x(x), y(y), z(z){}
};

void multiplica(float *m1, float *m2, float *m_resultado);

#endif