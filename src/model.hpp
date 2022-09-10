#ifndef ENGINE3D_MODEL
#define ENGINE3D_MODEL

#include <string>
#include <vector>
#include "utils.hpp"

using namespace std;


class Model{
private:
    float transform[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    vector<Point> vertex;
    vector<vector<int>> path;

    string loadFile(string path);
    void loadModel(string path);

public:
    Model(string path);
    vector<Point> getVertices();
    float* getTransform();
    void scale(float value);
    void rotate(float radian);
    void translate(float x, float y, float z);
};

#endif