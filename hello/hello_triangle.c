#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "../mat/mat.h"
#include "../game/gameobject.h"

static float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 2, // first triangle
    3, 4, 5, // second triangle
    0, 3, 2,
    0, 4, 3
    // 2, 1, 3,
    // 1, 0, 3
};
static int vertices_length[] = {
    3, 3, 2};
static float camera_speed;
static int firstMouse;
static float lastX;
static float lastY;
static float camera_pitch;
static float camera_yaw;
static int gameobjectID;
////////////
void dealwith_key(ST_Global *global_info)
{
    char keyPressed = global_info->keyPressed;
    printf("dealwith_key %c \n", global_info->keyPressed);
    ST_CUS_CAMERA *camera = global_info->camera;
    float camera_speed = 0.1f;
    if (keyPressed != 0)
    {
        if (keyPressed == 'W')
        {
            ((camera->camera_pos).element)[0] += ((camera->camera_front).element)[0] * camera_speed;
            ((camera->camera_pos).element)[1] += ((camera->camera_front).element)[1] * camera_speed;
            ((camera->camera_pos).element)[2] += ((camera->camera_front).element)[2] * camera_speed;
        }
        if (keyPressed == 'S')
        {
            ((camera->camera_pos).element)[0] -= ((camera->camera_front).element)[0] * camera_speed;
            ((camera->camera_pos).element)[1] -= ((camera->camera_front).element)[1] * camera_speed;
            ((camera->camera_pos).element)[2] -= ((camera->camera_front).element)[2] * camera_speed;
        }
        if (keyPressed == 'A')
        {
            ST_VEC3 *left_ = Vec3Cross(&(camera->camera_front), &(camera->camera_up));
            ST_VEC3_InPlace_Normalize(left_);
            ((camera->camera_pos).element)[0] -= (left_->element)[0] * camera_speed;
            ((camera->camera_pos).element)[1] -= (left_->element)[1] * camera_speed;
            ((camera->camera_pos).element)[2] -= (left_->element)[2] * camera_speed;
            Vec3Free(left_);
        }
        if (keyPressed == 'D')
        {
            ST_VEC3 *left_ = Vec3Cross(&(camera->camera_front), &(camera->camera_up));
            ST_VEC3_InPlace_Normalize(left_);
            ((camera->camera_pos).element)[0] += (left_->element)[0] * camera_speed;
            ((camera->camera_pos).element)[1] += (left_->element)[1] * camera_speed;
            ((camera->camera_pos).element)[2] += (left_->element)[2] * camera_speed;
            Vec3Free(left_);
        }
    }
}
void dealwith_mouse(ST_Global *global_info)
{
    printf("dealwith_mouse %f %f \n", global_info->mouseX, global_info->mouseY);

    if (firstMouse == 0)
    {
        lastX = global_info->mouseX;
        lastY = global_info->mouseY;
        firstMouse = 1;
    }

    float xoffset = global_info->mouseX - lastX;
    float yoffset = lastY - global_info->mouseY;
    lastX = global_info->mouseX;
    lastY = global_info->mouseY;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera_pitch += yoffset;
    camera_yaw += xoffset;

    if (camera_pitch > 89.0f)
        camera_pitch = 89.0f;
    if (camera_pitch < -89.0f)
        camera_pitch = -89.0f;
    ST_CUS_CAMERA *camera = global_info->camera;
    ((camera->camera_front).element)[0] = cos(RadiusOfDegree(camera_pitch)) * sin(RadiusOfDegree(camera_yaw));
    ((camera->camera_front).element)[1] = sin(RadiusOfDegree(camera_pitch));
    ((camera->camera_front).element)[2] = -cos(RadiusOfDegree(camera_pitch)) * cos(RadiusOfDegree(camera_yaw));
}
static ST_Gameobject *this;
static void update(ST_Global *global_info)
{
    dealwith_key(global_info);
    dealwith_mouse(global_info);
}
void InitTriangle()
{
    ST_Gameobject *gb = NewGameobject();
    this = gb;
    printf("mybegin  1\n");

    SetVerticesAndTriangle(gb, 48, vertices, 8, 3, vertices_length, sizeof(indices) / sizeof(unsigned int), indices);
    printf("mybegin  2\n");

    SetMaterial(gb, "hello/vertex_shader.glsl", "hello/fragment_shader.glsl", "resource/black_white.png");
    printf("mybegin  3\n");

    SetUpdate(gb, 0, update);
    printf("mybegin  4\n");

    SetDrawEnable(gb, 1);
    printf("mybegin  5\n");

    gameobjectID = RegisterGameobjectToGlobal(gb);
}