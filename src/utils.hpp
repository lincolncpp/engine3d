#ifndef ENGINE3D_UTILS
#define ENGINE3D_UTILS

struct Vertex{
    float x, y, z;
    Vertex(){}
    Vertex(float x, float y, float z) : x(x), y(y), z(z){}
};

struct Face{
    int a, b, c;
    Face(){}
    Face(int a, int b, int c) : a(a), b(b), c(c){}
};

void matrix_mul(float *a, float *b, float *c);

#endif