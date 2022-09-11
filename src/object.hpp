#ifndef ENGINE3D_OBJECT
#define ENGINE3D_OBJECT

#include <string>
#include <vector>
#include "utils.hpp"

using namespace std;

#define VERTEX_OPPOSITE 1<<0
#define VERTEX_HALF 1<<1
#define VERTEX_CENTRALIZE 1<<2


class Object{
private:
    int vertex_offset = 0;
    float transform[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    vector<Vertex> vertex;

    string loadFile(string path);
    void loadModel(string path);

public:
    Object(string path);
    vector<Vertex> getVertices();
    float* getTransform();

    void scale(float value);
    void rotate(float x, float y, float z);
    void translate(float x, float y, float z);

    void setVertexOffset(int value);
    int getVertexOffset();

    void Draw(GLint loc_transform, GLint loc_color);

    void VertexModifier(int modifier);
};

#endif