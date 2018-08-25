#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "gameobject.h"
#include "../mat/mat.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"
#include "../utils/file.h"

static ST_Global global_info;
ST_Global *return_global_info()
{
    return &global_info;
}
void draw_init()
{
    memset(&global_info, 0, sizeof(ST_Global));
    ST_CUS_CAMERA *camera = malloc(sizeof(ST_CUS_CAMERA));
    memset(camera, 0, sizeof(ST_CUS_CAMERA));
    (camera->camera_pos).element[0] = 0.0f;
    (camera->camera_pos).element[1] = 0.0f;
    (camera->camera_pos).element[2] = 2.0f;
    //
    (camera->camera_front).element[0] = 0.0f;
    (camera->camera_front).element[1] = 0.0f;
    (camera->camera_front).element[2] = -1.0f;
    //
    (camera->camera_up).element[0] = 0.0f;
    (camera->camera_up).element[1] = 1.0f;
    (camera->camera_up).element[2] = 0.0f;
    //
    camera->near_distance = 1.0f;
    camera->far_distance = 1000.0f;
    camera->near_long = 100.0f;
    camera->far_long = 10000.0f;
    ///////
    global_info.camera = camera;
}

// if you want to set your own camara
void SetMainCamera(ST_CUS_CAMERA *camera)
{
    free(global_info.camera);
    global_info.camera = camera;
}

ST_MAT4 *model(ST_Gameobject *gb)
{
    ST_MAT4 *transform = NewMat4(0);
    ST_MAT4 *old = transform;
    transform = D3_Rotate(transform, gb->x_rotate_degree, gb->y_rotate_degree, gb->z_rotate_degree);
    Mat4Free(old);
    // second : translate
    old = transform;
    transform = D3_Translate(transform, gb->x, gb->y, gb->z);
    Mat4Free(old);
    return transform;
}
ST_MAT4 *view(ST_Gameobject *gb) // lots about camera, so we put camera into global_info
{
    ST_CUS_CAMERA *camera_object = global_info.camera;
    ST_VEC3_Add_InplaceOP(&(camera_object->camera_pos), &(camera_object->camera_front), &(camera_object->camera_target));
    ST_MAT4 *viewT = D3_LookAtFrom(&(camera_object->camera_pos), &(camera_object->camera_target), &(camera_object->camera_up));
    return viewT;
}
ST_MAT4 *project(ST_Gameobject *gb)
{
    ST_CUS_CAMERA *camera_object = global_info.camera;

    ST_MAT4 *transform = NewMat4(0);
    ST_MAT4 *old = transform;
    transform = D3_Homoz(transform, camera_object->near_distance);
    Mat4Free(old);
    return transform;
}
ST_MAT4 *MVP(ST_Gameobject *gb)
{
    ST_MAT4 *M = model(gb);
    ST_MAT4 *V = view(gb);
    ST_MAT4 *P = project(gb);
    ST_MAT4 *VM = MatMat4(V, M);
    ST_MAT4 *PVM = MatMat4(P, VM);
    free(M);
    free(V);
    free(P);
    free(VM);
    return PVM;
}

void change_VAO(ST_Gameobject *gb)
{
    ST_Mesh *my_mesh = gb->mesh;
    glBindVertexArray(my_mesh->VAO);
}
void change_VBO(ST_Gameobject *gb)
{
    ST_Mesh *my_mesh = gb->mesh;
    glBindBuffer(GL_ARRAY_BUFFER, my_mesh->VBO);
}
void change_EBO(ST_Gameobject *gb)
{
    ST_Mesh *my_mesh = gb->mesh;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_mesh->EBO);
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
void ShaderSetMat4(void *self, char *name, float *value_list)
{
    ST_Shader *myshader = (ST_Shader *)self;
    unsigned int transformLoc = glGetUniformLocation(myshader->ID, name);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_list);
}
void do_draw(ST_Gameobject *gb, ST_MAT4 *mvp)
{
    change_VAO(gb);
    ST_Shader *myShader = (gb->material)->shader;
    glUseProgram(myShader->ID);
    ShaderSetMat4((void *)myShader, "transform", mvp->element);
    ST_Mesh *my_mesh = gb->mesh;
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
void CompileShader(ST_Gameobject *gb)
{

    ST_Shader *myshader = gb->material->shader;
    const char *vertex_shader = fileReadAll(gb->material->vertex_shader_path);
    const char *fragment_shader = fileReadAll(gb->material->fragment_shader_path);
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
void init_for_draw(ST_Gameobject *gb)
{
    // 1. shader
    CompileShader(gb);
    // 2. vertices
    ST_Mesh *my_mesh = gb->mesh;
    glGenBuffers(1, &(my_mesh->VBO));
    glGenVertexArrays(1, &(my_mesh->VAO));
    glGenBuffers(1, &(my_mesh->EBO));
    change_VAO(gb);
    change_VBO(gb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (my_mesh->vertices_num), my_mesh->vertices, GL_STATIC_DRAW);
    // 3.1 then set our vertex attributes pointers
    int leiji = 0;
    for (int location = 0; location != my_mesh->vertex_location_num; location++)
    {
        glVertexAttribPointer(location, (my_mesh->vertex_length)[location], GL_FLOAT, GL_FALSE, my_mesh->vertex_step * sizeof(float), (void *)(leiji));
        glEnableVertexAttribArray(location);
        leiji += (my_mesh->vertex_length)[location];
    }
    // 3.2 set EBO which deal with the triangles
    change_EBO(gb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * my_mesh->indices_num, my_mesh->indices, GL_STATIC_DRAW);
    // 3.3 texture
    ST_Material *my_materia = gb->material;
    glGenTextures(1, &(my_materia->TBO));
    glBindTexture(GL_TEXTURE_2D, my_materia->TBO);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    //set the (0,0) on the left button point
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(my_materia->texture_path, &width, &height, &nrChannels, 0);
    if (data)
    {
        printf("image:%d__%d\n", width, height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("fail_to_create_texture_:%s_not_found\n", my_materia->texture_path);
        usleep(1000000000);
    }
    stbi_image_free(data);
}
static ST_VEC4 fortest;
void gameobject_draw(ST_Gameobject *gb)
{
    if (gb->draw_enable == 0)
    {
        return;
    }
    if (gb->draw_prepared == 0)
    {
        // init something about drawing
        init_for_draw(gb);
        gb->draw_prepared = 1;
    }
    // calculate the mvp
    ST_MAT4 *mvp = MVP(gb);
    // change the opengl buffer context, and do the drawing
    do_draw(gb, mvp);
    // don't forget free the mvp
    printf("themvp:-\n");
    PrintMat4(mvp);
    (fortest.element)[0] = 1.0f;
    (fortest.element)[1] = 1.0f;
    (fortest.element)[2] = 1.0f;
    (fortest.element)[3] = 1.0f;
    MatVec4_Inplace(mvp, &fortest);
    PrintVec4(&fortest);
    free(mvp);
}
