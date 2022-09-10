#include <GL/glew.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "model.hpp"
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

 
int main(){
 
    // Initializing GLFW
    glfwInit();

    // Setting the window to start invisible
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Creating GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "SCC0250 - Trabalho 1", NULL, NULL);

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
 



    Model* pain = new Model("models/teapot.data");
    vector<Point> v = pain->getVertices();

    int num_vertices = v.size();
    Point vertices[num_vertices]; 

    for(int i = 0;i < num_vertices;i++){
        vertices[i] = v[i];
        vertices[i].x /= 4.f;
        vertices[i].y /= 4.f;
        vertices[i].z /= 4.f;
    }


    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
 

    // Associando variávels do programa GLSL (Fragment Shader) para definir cores
    GLint loc_color = glGetUniformLocation(program, "color");

    GLint loc_transform = glGetUniformLocation(program, "mat_transformation");


    // Exibindo nossa janela
    glfwShowWindow(window);


    glEnable(GL_DEPTH_TEST);// ### importante para 3D

    while (!glfwWindowShouldClose(window))
    {



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
        multiplica(mat_rotation_y,mat_transformation, mat_transformation);

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        // enviando a matriz de transformacao para a GPU
        glUniformMatrix4fv(loc_transform, 1, GL_TRUE, mat_transformation);

        
        for(int triangle=0; triangle < num_vertices; triangle=triangle+3){
            
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
