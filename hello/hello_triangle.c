#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "hello.h"
#include "../utils/file.h"
#include "../utils/utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

float vertices[] = {
    -0.2f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.2f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.2f, -0.2f, -0.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.2f, 0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    0, 1, 2, // second triangle
    0, 3, 2,
    0, 2, 3
    // 2, 1, 3,
    // 1, 0, 3
};
ST_VEC4 test_vec4;
void refreshtest_vec4()
{
    test_vec4.element[0] = 1.0f;
    test_vec4.element[1] = 1.0f;
    test_vec4.element[2] = 1.0f;
    test_vec4.element[3] = 1.0f;
}

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
void ShaderSetMat4(void *self, char *name, float *value_list)
{
    ST_Shader *myshader = (ST_Shader *)self;
    unsigned int transformLoc = glGetUniformLocation(myshader->ID, name);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_list);
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
    myShader->setMat4 = ShaderSetMat4;
    return myShader;
}
void MeshSetAllContext(void *self, char *image)
{
    ST_Mesh *my_mesh = (ST_Mesh *)self;
    if (my_mesh->set == 1)
    {
        return;
    }
    my_mesh->set = 1;
    glGenBuffers(1, &(my_mesh->VBO));
    glGenVertexArrays(1, &(my_mesh->VAO));
    glGenBuffers(1, &(my_mesh->EBO));

    glBindBuffer(GL_ARRAY_BUFFER, my_mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (my_mesh->vertices_num), my_mesh->vertices, GL_STATIC_DRAW);

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
        printf("fail_to_create_texture_:%s_not_found\n", image);
        usleep(1000000000);
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
    myMesh->setAll((void *)myMesh, "resource/dota2.png");
    // last. config is set, you can do render in the main loop
}

////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
int jiou = 0;
ST_MAT4 *transform;
ST_Gameobject *camera;
ST_CUS_CAMERA *camera_cus;
ST_Gameobject *myobject;
char keyPressed;
float lastX;
float lastY;
double mouse_xpos;
double mouse_ypos;
int firstMouse;
void PrintAll(char *what) {
    printf("%s:--\n", what);
    PrintMat4(transform);
    refreshtest_vec4();
    MatVec4_Inplace(transform, &test_vec4);
    PrintVec4(&test_vec4);
}
void model_t()
{
    if (myobject == 0)
    {
        myobject = malloc(sizeof(ST_Gameobject));
        memset(myobject, 0, sizeof(ST_Gameobject));
    }
    float timeValue = glfwGetTime();
    // myobject->x = cos(timeValue);
    // myobject->y = sin(timeValue);
    // printf("myobject->y %f\n", myobject->y);
    // myobject->x_rotate_degree =  30.0f;
    // myobject->y_rotate_degree = sin(timeValue*0.06) * 360.0f;
    // myobject->z = 1.1f;
    // first : rotation
    ST_MAT4 *old = transform;
    transform = D3_Rotate(transform, myobject->x_rotate_degree, myobject->y_rotate_degree, myobject->z_rotate_degree);
    Mat4Free(old);
    // second : translate
    old = transform;
    transform = D3_Translate(transform, myobject->x, myobject->y, myobject->z);
    Mat4Free(old);
}
ST_VEC3 camera_pos;   // 相机位置
ST_VEC3 camera_front; // 相机看的方向
ST_VEC3 camera_up;    // should be constant 人工选定的世界坐标下的 up
float camera_speed;
float camera_pitch;
float camera_yaw;
void dealwith_key()
{
    printf("the key is %c \n", keyPressed);
    if (keyPressed != 0)
    {
        if (keyPressed == 'W')
        {
            (camera_pos.element)[0] += (camera_front.element)[0] * camera_speed;
            (camera_pos.element)[1] += (camera_front.element)[1] * camera_speed;
            (camera_pos.element)[2] += (camera_front.element)[2] * camera_speed;
        }
        if (keyPressed == 'S')
        {
            (camera_pos.element)[0] -= (camera_front.element)[0] * camera_speed;
            (camera_pos.element)[1] -= (camera_front.element)[1] * camera_speed;
            (camera_pos.element)[2] -= (camera_front.element)[2] * camera_speed;
        }
        if (keyPressed == 'A')
        {
            ST_VEC3 *left_ = Vec3Cross(&camera_front, &camera_up);
            ST_VEC3_InPlace_Normalize(left_);
            (camera_pos.element)[0] -= (left_->element)[0] * camera_speed;
            (camera_pos.element)[1] -= (left_->element)[1] * camera_speed;
            (camera_pos.element)[2] -= (left_->element)[2] * camera_speed;
            Vec3Free(left_);
        }
        if (keyPressed == 'D')
        {
            ST_VEC3 *left_ = Vec3Cross(&camera_front, &camera_up);
            ST_VEC3_InPlace_Normalize(left_);
            (camera_pos.element)[0] += (left_->element)[0] * camera_speed;
            (camera_pos.element)[1] += (left_->element)[1] * camera_speed;
            (camera_pos.element)[2] += (left_->element)[2] * camera_speed;
            Vec3Free(left_);
        }
    }
}
void dealwith_mouse(double xpos, double ypos)
{
    if (firstMouse == 0)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 1;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera_pitch += yoffset;
    camera_yaw += xoffset;

    if (camera_pitch > 89.0f)
        camera_pitch = 89.0f;
    if (camera_pitch < -89.0f)
        camera_pitch = -89.0f;

    (camera_front.element)[0] = cos(RadiusOfDegree(camera_pitch)) * sin(RadiusOfDegree(camera_yaw));
    (camera_front.element)[1] = sin(RadiusOfDegree(camera_pitch));
    (camera_front.element)[2] = -cos(RadiusOfDegree(camera_pitch)) * cos(RadiusOfDegree(camera_yaw));
}
void view_t()
{
    if (camera == 0)
    {
        camera = malloc(sizeof(ST_Gameobject));
        memset(camera, 0, sizeof(ST_Gameobject));
        camera_cus = malloc(sizeof(ST_CUS_CAMERA));
        memset(camera_cus, 0, sizeof(ST_CUS_CAMERA));
        camera->custom = camera_cus;
        camera_cus->near_distance = 1.0f;
        camera_cus->far_distance = 1000.0f;
        camera_cus->near_long = 100.0f;
        camera_cus->far_long = 10000.0f;
        //
        (camera_pos.element)[0] = 0.0f;
        (camera_pos.element)[1] = 0.0f;
        (camera_pos.element)[2] = 2.0f;
        //
        (camera_front.element)[0] = 0.0f;
        (camera_front.element)[1] = 0.0f;
        (camera_front.element)[2] = -1.0f;
        //
        (camera_up.element)[0] = 0.0f;
        (camera_up.element)[1] = 1.0f;
        (camera_up.element)[2] = 0.0f;
        //
        camera_speed = 0.1f;
        //
        camera_pitch = 0.0f;
        camera_yaw = 0.0f;
    }
    float timeValue = glfwGetTime();

    dealwith_mouse(mouse_xpos, mouse_ypos);
    dealwith_key();
    //

    ST_VEC3 *camera_target = ST_VEC3_Add(&camera_pos, &camera_front);
    ST_MAT4 *viewT = D3_LookAtFrom(&camera_pos, camera_target, &camera_up);
    Vec3Free(camera_target);
    
    ST_MAT4 *old = transform;
    transform = MatMat4(viewT, old);
    Mat4Free(viewT);
    Mat4Free(old);
}
void projection_t()
{
    // projectio is relative to camera
    if (camera == 0)
    {
        camera = malloc(sizeof(ST_Gameobject));
        memset(camera, 0, sizeof(ST_Gameobject));
        camera_cus = malloc(sizeof(ST_CUS_CAMERA));
        memset(camera_cus, 0, sizeof(ST_CUS_CAMERA));
        camera->custom = camera_cus;
        camera_cus->near_distance = 1.0f;
        camera_cus->far_distance = 1000.0f;
        camera_cus->near_long = 100.0f;
        camera_cus->far_long = 10000.0f;
    }
    // z --> (0, 1) || x --> (-1 , 1) || y --> (-1, 1)
    PrintAll("beforeHomo");
    float timeValue = glfwGetTime();
    ST_MAT4 *old = transform;
    transform = D3_Homoz(transform, camera_cus->near_distance);
    Mat4Free(old);
    PrintAll("afterHomo");

    // old = transform;
    // transform = D3_Scale(transform, 1.0f / camera_cus->near_long, 1.0f / camera_cus->near_long, 1.0f);
    // Mat4Free(old);
    // float k = -(1.0f / (camera_cus->far_distance - camera_cus->near_distance));
    // printf("k-->%f\n", k);
    // float b = k * camera_cus->near_distance;
    // z should scale and then translate
    // old = transform;
    // transform = D3_Scale(transform, 1.0f, 1.0f, k);
    // Mat4Free(old);
    // PrintAll("after_z_scale");

    // old = transform;
    // transform = D3_Translate(transform, 0.0f, 0.0f, b);
    // Mat4Free(old);
    // PrintAll("after_z_translate");
}
void modify()
{
    Mat4SetValue(transform, 4, 4, 0.1f);
}
void draw_triangle(char key_press, double xpos, double ypos)
{
    keyPressed = 0;
    keyPressed = key_press;
    mouse_xpos = xpos;
    mouse_ypos = ypos;
    jiou++;
    refreshtest_vec4();
    if (transform == 0)
    {
        transform = NewMat4(0);
    }
    else
    {
        SetMat4Identity(transform);
    }
    model_t();
    view_t();
    projection_t();
    // modify();
    prepare_draw_triangle();
    // float timeValue = glfwGetTime();
    // float change = sin(3.141592f / 2.0f);
    // transform = MatMat4(transform, transform);
    // (transform->element)[1] = change;
    myShader->use((void *)myShader);
    myShader->setMat4((void *)myShader, "transform", transform->element);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    // modify();
    // myShader->setMat4((void *)myShader, "transform", transform->element);

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
