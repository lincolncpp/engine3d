#include <GL/glew.h>  
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "object.hpp"

using namespace std;


string Object::loadFile(string path){
    // Loading file
    ifstream file(path);
    if (file.is_open()){

        // Getting data from file
        string in = "";
        string buffer;
        while(getline(file, buffer)){
            in += buffer + " ";
        }

        // Closing file
        file.close();

        return in;
    }
    else{
        cout << "Can't open file " << path << endl;
    }

    return "";
}

void Object::loadModel(string path){
    // Loading model data from file
    string data = loadFile(path);

    // Send model data to cpp input buffer
    istringstream input(data);
    cin.rdbuf(input.rdbuf());
    
    // Reading all model vertices
    int vertex_count;
    cin >> vertex_count;
    for(int i = 0;i < vertex_count;i++){
        float x, y, z;
        cin>>x>>y>>z;

        vertex.push_back(Point(x, y, z));
    }
}

Object::Object(string data){
    loadModel(data);
}

vector<Point> Object::getVertices(){
    return vertex;
}

float* Object::getTransform(){
    return transform;
}

void Object::scale(float value){
    if (abs(value) > 1e-8){
        float scale[16] = {
            value, 0.0f, 0.0f, 0.0f,
            0.0f ,value, 0.0f, 0.0f,
            0.0f , 0.0f,value, 0.0f,
            0.0f , 0.0f, 0.0f, 1.0f,
        };
        
        matrix_mul(transform, scale, transform);
    }
}

void Object::rotate(float x, float y, float z){
    // X-axis rotation
    if (abs(x) > 1e-8){
        float c = cos(x);
        float s = sin(x);
        float rotx[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  c  ,  -s , 0.0f,
            0.0f,  s  ,   c , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix_mul(transform, rotx, transform);
    }

    // Y-axis rotation
    if (abs(y) > 1e-8){
        float c = cos(y);
        float s = sin(y);
        float roty[16] = {
            c   , 0.0f,   s , 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s  , 0.0f,   c , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix_mul(transform, roty, transform);
    }

    // Z-axis rotation
    if (abs(z) > 1e-8){
        float c = cos(z);
        float s = sin(z);
        float rotz[16] = {
            c   , -s  , 0.0f, 0.0f,
            s   ,  c  , 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        matrix_mul(transform, rotz, transform);
    }
}
void Object::translate(float x, float y, float z){
    // Need impĺementation
}

void Object::setVertexOffset(int value){
    vertex_offset = value;
}

int Object::getVertexOffset(){
    return vertex_offset;
}

void Object::Draw(GLint loc_transform, GLint loc_color){
    
    glUniformMatrix4fv(loc_transform, 1, GL_TRUE, transform);

        
    for(int tri = 0; tri < vertex.size(); tri=tri+3){
        
        srand(vertex_offset + tri);
        float R = (float)rand() / (float)RAND_MAX ;
        float G = (float)rand() / (float)RAND_MAX ;
        float B = (float)rand() / (float)RAND_MAX ;

        glUniform4f(loc_color, R, G, B, 1);
        glDrawArrays(GL_TRIANGLES, vertex_offset + tri, 3);
    }
}

