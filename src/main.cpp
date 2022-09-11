#include <GL/glew.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "object.hpp"
#include "utils.hpp"

using namespace std;
 


float angulo = 0.0f;
float angulo_inc = 0.5f;

float c = 1.0f; // armazenar cosseno (da matriz de rotacao)
float s = 0.0f; // armazenar seno  (da matriz de rotcao)

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
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
        cout << "E pressed" << endl;
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


    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);



        angulo += angulo_inc;



        // definindo a matriz de rotacao (na realidade eh um vetor, mas o OpenGl processa como matriz 4x4)
        c = cos( ((angulo) * M_PI / 180.0) ); // cos considerando conversao para radianos
        s = sin( ((angulo) * M_PI / 180.0) );

        float mat_rotation_z[16] = {
            c   , -s  , 0.0f, 0.0f,
            s   ,  c  , 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_rotation_x[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  c  ,  -s , 0.0f,
            0.0f,  s  ,   c , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_rotation_y[16] = {
            c   , 0.0f,   s , 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s  , 0.0f,   c , 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_transformation[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };


        // multiplica(mat_rotation_z,mat_rotation_x, mat_transformation);
        matrix_mul(mat_rotation_y,mat_transformation, mat_transformation);

        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        // enviando a matriz de transformacao para a GPU
        glUniformMatrix4fv(loc_transform, 1, GL_TRUE, mat_transformation);

        
        for(int triangle=0; triangle < object_vertices; triangle=triangle+3){
            
            srand(triangle); // definir mesma semente aleatoria para cada triangulo
            float R = (float)rand() / (float)RAND_MAX ;
            float G = (float)rand() / (float)RAND_MAX ;
            float B = (float)rand() / (float)RAND_MAX ;

            glUniform4f(loc_color, R, G, B, 1);
            glDrawArrays(GL_TRIANGLES, triangle, 3);
        }

    
        glfwSwapBuffers(window);
        
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
