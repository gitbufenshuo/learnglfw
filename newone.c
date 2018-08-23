#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "utils/math.h"
#include "utils/time.h"
#include "hello/hello.h"
//
int global_count;
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    printf("%s__%d\n", "resize", global_count);
    global_count++;
    glViewport(0, 0, width, height);
}

static char keyP;
//
void processInput(GLFWwindow *window)
{
    keyP = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        printf("you input 9\n");
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        keyP = 'W';
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        keyP = 'S';
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        keyP = 'A';
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        keyP = 'D';
    }
}

//
time_t ts;
void timestamp()
{
    time(&ts);
    printf("timeis:%ld\n", ts);
}

int main(void)
{
    GLFWwindow *window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        printf("glfwCreateWindow_error\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // sleep
        usleep(28000);
        // get frame per second
        int fps = getfps();
        if (fps > 0)
        {
            printf("fps:%d\n", fps);
        }
        // process input
        processInput(window);

        /* Render here */
        change_smooth(0.01f);
        draw_triangle(keyP);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
