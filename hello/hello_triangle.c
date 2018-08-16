#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "hello.h"
#include "../utils/file.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f, 4.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 4.0f};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    // 2, 1, 3,
    // 1, 0, 3
};
void CompileShader(void *self, char *vs, char *fs)
{
    ST_Shader *myshader = (ST_Shader *)self;
    const char *vertex_shader = fileReadAll(vs);
    const char *fragment_shader = fileReadAll(fs);
    // vertex
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("vvv--%s\n", infoLog);
    }
    // fragment
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    int success1;
    char infoLog1[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success1);
    if (!success1)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog1);
        printf("fff--%s\n", infoLog1);
    }
    //
    myshader->ID = glCreateProgram();
    glAttachShader(myshader->ID, vertexShader);
    glAttachShader(myshader->ID, fragmentShader);
    glLinkProgram(myshader->ID);
    // delete shader because we have the generated program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
void UseShader(void *self)
{
    ST_Shader *myshader = (ST_Shader *)self;
    glUseProgram(myshader->ID);
}

void ShaderSetInt(void *self, char *name, int value)
{
    ST_Shader *myshader = (ST_Shader *)self;
    glUniform1i(glGetUniformLocation(myshader->ID, name), value);
}
void ShaderSetFloat(void *self, char *name, float value)
{
    ST_Shader *myshader = (ST_Shader *)self;
    glUniform1f(glGetUniformLocation(myshader->ID, name), value);
}

ST_Shader *myShader;
ST_Shader *NewST_Shader()
{
    myShader = malloc(sizeof(ST_Shader));
    memset(myShader, 0, sizeof(ST_Shader));
    myShader->compile = CompileShader;
    myShader->use = UseShader;
    myShader->setInt = ShaderSetInt;
    myShader->setFloat = ShaderSetFloat;
    return myShader;
}
void MeshSetAllContext(void* self, char* image){
    ST_Mesh *my_mesh = (ST_Mesh*)self;
    if (my_mesh->set == 1) {
        return;
    }
    my_mesh->set = 1;
    glGenBuffers(1, &(my_mesh->VBO));
    glGenVertexArrays(1, &(my_mesh->VAO));
    glGenBuffers(1, &(my_mesh->EBO));

    glBindBuffer(GL_ARRAY_BUFFER, my_mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(my_mesh->vertices_num), my_mesh->vertices, GL_STATIC_DRAW);

    //
    glBindVertexArray(my_mesh->VAO);
    // 3.1 then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * 4));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * 4));
    glEnableVertexAttribArray(2);
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * my_mesh->indices_num, my_mesh->indices, GL_STATIC_DRAW);
    /////////////////// texture
    glGenTextures(1, &(my_mesh->TBO));
    glBindTexture(GL_TEXTURE_2D, my_mesh->TBO);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    //set the (0,0) on the left button point
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(image, &width, &height, &nrChannels, 0);
    if (data)
    {
        printf("image:%d__%d\n", width, height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("fail to create texture");
    }
    stbi_image_free(data);

}
ST_Mesh *myMesh;
ST_Mesh *NewST_Mesh()
{
    myMesh = malloc(sizeof(ST_Mesh));
    memset(myMesh, 0, sizeof(ST_Mesh));
    myMesh->vertices = vertices;
    myMesh->vertices_num = sizeof(vertices);
    myMesh->indices = indices;
    myMesh->indices_num = sizeof(indices);
    myMesh->setAll = MeshSetAllContext;
    return myMesh;
}

int prepared = 0;
void prepare_draw_triangle()
{
    if (prepared == 1)
    {
        return;
    }
    prepared = 1;
    // GEN THE SHADER PROGRAM AND USE IT
    myShader = NewST_Shader();
    myShader->compile((void *)myShader, "./hello/vertex_shader.glsl", "./hello/fragment_shader.glsl");
    // GEN VBO VAO
    myMesh = NewST_Mesh();
    myMesh->setAll((void*)myMesh, "./learng.jpg");
    // last. config is set, you can do render in the main loop
}
int jiou = 0;
void draw_triangle()
{
    jiou++;
    prepare_draw_triangle();
    float timeValue = glfwGetTime();
    float change = ((sin(timeValue) / 2.0f) + 0.5f) * 0.0f;
    myShader->use((void *)myShader);
    myShader->setFloat((void *)myShader, "change", change);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
