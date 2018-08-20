#include <GLFW/glfw3.h>
#include "../utils/math.h"
float r = 0.01;
void change_smooth(float step)
{
    r = plus_unit(r, step);
    glClearColor(r, r, r, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}