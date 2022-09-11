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
    

    // .obj structure
    vector<Vertex> vertices;
    vector<Face> faces;


    // Reading .obj file
    string cmd;
    while(cin>>cmd){
        // Reading vertex
        if (cmd.compare("v") == 0){
            Vertex p;
            cin>>p.x>>p.y>>p.z;

            vertices.push_back(p);
        }

        // Reading face
        if (cmd.compare("f") == 0){
            Face f;
            string s;
            cin>>f.a>>s;
            cin>>f.b>>s;
            cin>>f.c>>s;

            // Converting to base-0
            f.a--;
            f.b--;
            f.c--;

            faces.push_back(f);
        }
    }

    // Creating ordered vertices
    for(Face &f:faces){
        vertex.push_back(vertices[f.a]);
        vertex.push_back(vertices[f.b]);
        vertex.push_back(vertices[f.c]);
    }
}

Object::Object(string data){
    loadModel(data);    
}

vector<Vertex> Object::getVertices(){
    return vertex;
}

float* Object::getTransform(){
    return transform;
}

void Object::scale(float value){
    if (abs(value) > 1e-8){ // Checking if it's not zero
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
    if (abs(x) > 1e-8){ // Checking if it's not zero
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
    if (abs(y) > 1e-8){ // Checking if it's not zero
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
    if (abs(z) > 1e-8){ // Checking if it's not zero
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
    if (abs(x)+abs(y)+abs(z) > 1e-8){ // Checking if it's not zero
        float translate[16] = {
            1.0f , 0.0f, 0.0f,  x  ,
            0.0f , 1.0f, 0.0f,  y  ,
            0.0f , 0.0f, 1.0f,  z  ,
            0.0f , 0.0f, 0.0f, 1.0f,
        };
        
        matrix_mul(transform, translate, transform);
    }
}

void Object::setVertexOffset(int value){
    vertex_offset = value;
}

int Object::getVertexOffset(){
    return vertex_offset;
}

void Object::Draw(GLint loc_transform, GLint loc_color){
    
    // Sending object matrix to GPU
    glUniformMatrix4fv(loc_transform, 1, GL_TRUE, transform);

    // Drawing triangles
    for(int tri = 0; tri < vertex.size(); tri=tri+3){
        
        srand(vertex_offset + tri);
        float rnd = (float)rand() / (float)RAND_MAX ;
        float R = rnd;
        float G = 0.5f;
        float B = 0.5f;

        glUniform4f(loc_color, R, G, B, 1);
        glDrawArrays(GL_TRIANGLES, vertex_offset + tri, 3);
    }
}

void Object::VertexModifier(int modifier){
    // Setting vertex (x, y, z) to (-x, -y, -z)
    if (modifier&VERTEX_OPPOSITE){
        for(Vertex &p:vertex){
            p.x *= -1;
            p.y *= -1;
            p.z *= -1;
        }
    }

    // Setting vertex (x, y, z) to (x/2, y/2, z/2)
    if (modifier&VERTEX_HALF){
        for(Vertex &p:vertex){
            p.x *= 0.5f;
            p.y *= 0.5f;
            p.z *= 0.5f;
        }
    }

    // Fixing model center
    if (modifier&VERTEX_CENTRALIZE){
        float maxx = -1e9;
        float minx = 1e9;
        float maxy = -1e9;
        float miny = 1e9;
        float maxz = -1e9;
        float minz = 1e9;
        for(Vertex &p:vertex){
            maxx = max(maxx, p.x);
            minx = min(minx, p.x);
            maxy = max(maxy, p.y);
            miny = min(miny, p.y);
            maxz = max(maxz, p.z);
            minz = min(minz, p.z);
        }

        float cx = (maxx+minx)/2.f;
        float cy = (maxy+miny)/2.f;
        float cz = (maxz+minz)/2.f;
        for(Vertex &p:vertex){
            p.x -= cx;
            p.y -= cy;
            p.z -= cz;
        }
    }
}