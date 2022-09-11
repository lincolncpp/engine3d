#include <GL/glew.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "object.hpp"
#include "utils.hpp"

using namespace std;

// Vertex Shader GLSL code 
const GLchar* vertex_code =
    "attribute vec3 position;\n"
    "uniform mat4 mat_transformation;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mat_transformation * vec4(position, 1.0);\n"
    "}\n";

// GLSL para Fragment Shader
const GLchar* fragment_code =
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"   
    "    gl_FragColor = color;\n"
    "}\n";
 
 // Scene objects
 vector<Object*> objects;
 int selected_object = 0;



void compileShader(GLuint shader){
    // Compiling shader
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        // Retrieving error log length
        int infoLength = 512;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

        // Retrieving error log message
        char info[infoLength];
        glGetShaderInfoLog(shader, infoLength, NULL, info);

        cout << "Shader compilation error" << endl;
        cout << info << endl;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    // Change current object
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        selected_object++;
        selected_object %= objects.size();

        cout << "Object #" << selected_object << " > " << "Selected" << endl;
    }

    // Rotate -y (key RIGHT)
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->rotate(0, -M_PI / 10.0f, 0);
        cout << "Object #" << selected_object << " > " << "Rotate(0, -π/10, 0)"  << endl;
    }
    // Rotate y (key LEFT)
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->rotate(0, M_PI / 10.0f, 0);
        cout << "Object #" << selected_object << " > " << "Rotate(0, π/10, 0)"  << endl;
    }
    // Rotate -z (key UP)
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->rotate(0, 0, -M_PI / 10.0f);
        cout << "Object #" << selected_object << " > " << "Rotate(-π/10, 0, 0)"  << endl;
    }
    // Rotate z (key DOWN)
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->rotate(0, 0, M_PI / 10.0f);
        cout << "Object #" << selected_object << " > " << "Rotate(π/10, 0, 0)"  << endl;
    }

    // Scale up (key E)
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->scale(1.1f);
        cout << "Object #" << selected_object << " > " << "Scale(1.1)"  << endl;
    }
    // Scale down (key Q)
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->scale(0.9f);
        cout << "Object #" << selected_object << " > " << "Scale(0.9)"  << endl;
    }

    // Translate up (key W)
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->translate(0, 0.1f, 0);
        cout << "Object #" << selected_object << " > " << "Translate(0, 0.1, 0)"  << endl;
    }
    // Translate down (key S)
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->translate(0, -0.1f, 0);
        cout << "Object #" << selected_object << " > " << "Translate(0, -0.1, 0)"  << endl;
    }
    // Translate right (key D)
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->translate(0.1f, 0, 0);
        cout << "Object #" << selected_object << " > " << "Translate(0.1, 0, 0)"  << endl;
    }
    // Translate right (key A)
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        objects[selected_object]->translate(-0.1f, 0, 0);
        cout << "Object #" << selected_object << " > " << "Translate(-0.1, 0, 0)"  << endl;
    }

}
 
int main(){
 
    // Initializing GLFW
    glfwInit();

    // Setting the window to start invisible
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Creating GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "3DEngine", NULL, NULL);

    // Making the context of the window for the calling thread
    glfwMakeContextCurrent(window);

    // Initializing GLEW
    GLint GlewInitResult = glewInit();
    cout << "GlewStatus: " << glewGetErrorString(GlewInitResult) << endl;

    // Creating the program and the shaders
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Attaching GLSL codes to the shaders
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compiling shaders
    compileShader(vertex);
    compileShader(fragment);

    // Attaching shaders to program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Linking program
    glLinkProgram(program);
    glUseProgram(program);
 
    // Loading objects 3d
    objects.push_back(new Object("models/teapot.data"));
    objects.push_back(new Object("models/teapot.data"));
    objects.push_back(new Object("models/teapot.data"));
    objects.push_back(new Object("models/teapot.data"));
    objects.push_back(new Object("models/teapot.data"));

    // Loading vertices from all objects
    vector<Point> vertex_all;
    for(int i = 0;i < objects.size();i++){
        // Setting object buffer offset
        objects[i]->setVertexOffset(vertex_all.size());

        // Inserting object vertices into vertex_all
        vector<Point> vertex = objects[i]->getVertices();
        vertex_all.insert(vertex_all.end(), vertex.begin(), vertex.end());
    }

    // Converting vector to array
    int object_vertices = vertex_all.size();
    Point vertices[object_vertices];
    for(int i = 0;i < object_vertices;i++){
        vertices[i] = vertex_all[i];
    }

    // Creating vertices buffer
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Sending all vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Getting GLSL position id
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
 
    // Getting GLSL color id
    GLint loc_color = glGetUniformLocation(program, "color");

    // Getting GLSL mat_transformation id
    GLint loc_transform = glGetUniformLocation(program, "mat_transformation");

    // Showing GLFW window
    glfwShowWindow(window);

    // Enabling GL_DEPTH_TEST
    glEnable(GL_DEPTH_TEST);
    
    // Setting input callback
    glfwSetKeyCallback(window, key_callback);

    int i = 0;
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2, 0.2, 0.2, 1.0);

        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        // Drawing selected object
        objects[selected_object]->Draw(loc_transform, loc_color);
    
        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
