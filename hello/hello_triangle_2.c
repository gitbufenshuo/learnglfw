#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "../mat/mat.h"
#include "../game/gameobject.h"

// vertex_pos[3], vertex_color[3], uv[2]
static float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
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

static int gameobjectID;
////////////

static ST_Gameobject *this;
static void update(ST_Global *global_info)
{
    // printf("update is called\n");
}
void InitTriangle2()
{
    ST_Gameobject *gb = NewGameobject();
    this = gb;
    gb->x = -1.0f;
    gb->y = -1.0f;
    gb->z = -1.0f;
    printf("%x_mybegin2  1\n", gb);

    SetVerticesAndTriangle(gb, 48, vertices, 8, 3, vertices_length, sizeof(indices) / sizeof(unsigned int), indices);
    printf("mybegin2  2\n");

    SetMaterial(gb, "hello/vertex_shader.glsl", "hello/fragment_shader.glsl", "resource/black_white.png", GL_CLAMP_TO_BORDER);
    printf("mybegin2  3\n");

    SetUpdate(gb, 0, update);
    printf("mybegin2  4\n");

    SetDrawEnable(gb, 1);
    printf("mybegin2  5\n");

    gameobjectID = RegisterGameobjectToGlobal(gb);
    for (int numout = 10; numout != 0; numout--)
    {
        for (int num = 10; num != 0; num--)
        {
            for (int numin = 10; numin != 0; numin--)
            {
                ST_Gameobject *new_one = shallow_clone(gb);
                new_one->z = (float)numout * -1.0f;
                new_one->x = (float)num * -1.0f;
                new_one->y = (float)numin * -1.0f;
                new_one->draw_prepared = 1;
                RegisterGameobjectToGlobal(new_one);
            }
        }
    }
}