#include <iostream>
#include <fstream>
#include <sstream>
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
    // Need impĺementation
}

void Object::rotate(float radian){
    // Need impĺementation
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