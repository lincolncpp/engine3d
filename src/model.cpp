#include <iostream>
#include <fstream>
#include <sstream>
#include "model.hpp"

using namespace std;


string Model::loadFile(string path){
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

void Model::loadModel(string path){
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

Model::Model(string data){
    loadModel(data);
    
}

vector<Point> Model::getVertices(){
    return vertex;
}