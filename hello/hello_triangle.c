#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include  <stdio.h>
#include "../utils/file.h"
float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.8f,
    0.5f, -0.5f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.8f,
    -0.5f, -0.5f, 0.0f, 0.1f,
    0.5f, -0.5f, 0.0f, 0.1f,
    0.0f, 0.5f, 0.0f, 0.1f,
    0.0f, 0.0f, 0.0f, 0.1f};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 2, // first triangle
    1, 2, 3  // second triangle
};
unsigned int shaderProgram;
void compile()
{
    const char *vertex_shader = fileReadAll("./hello/vertex_shader.glsl");
    printf("ver-->%s\n", vertex_shader);
    const char *fragment_shader = fileReadAll("./hello/fragment_shader.glsl");
    printf("ver-->%s\n", fragment_shader);
    // vertex
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("vvv--%s\n",infoLog);
    }
    // fragment
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    int  success1;
    char infoLog1[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success1);
    if(!success1)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog1);
        printf("fff--%s\n",infoLog1);
    }
    //
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // delete shader because we have the generated program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
unsigned int VBO[] = {0, 0};
unsigned int VAO[] = {0, 0};
unsigned int EBO[] = {0, 0};
int prepared = 0;
void prepare_draw_triangle()
{
    if (prepared == 1)
    {
        return;
    }
    prepared = 1;
    // GEN THE SHADER PROGRAM AND USE IT
    compile();
    glUseProgram(shaderProgram);
    // GEN VBO VAO
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, EBO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 16 * 4, vertices, GL_STATIC_DRAW);
    // 3.0 bind VAO should be here before set vertex attribute
    glBindVertexArray(VAO[0]);
    // 3.1 then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(12));
    glEnableVertexAttribArray(1);
    // 3.2 the EBO set
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12, indices, GL_STATIC_DRAW);

    // 2. copy our vertices array in a buffer for OpenGL to use
    // glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // glBufferData(GL_ARRAY_BUFFER, 12 * 4, vertices + 16, GL_STATIC_DRAW);
    // 3.0 bind VAO should be here before set vertex attribute
    glBindVertexArray(VAO[1]);
    // 3.1 then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(12));
    glEnableVertexAttribArray(1);
    // 3.3 the EBO set
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12, indices + 3, GL_STATIC_DRAW); 

    // last. config is set, you can do render in the main loop
}
int jiou = 0;
void draw_triangle()
{
    jiou++;
    prepare_draw_triangle();
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
