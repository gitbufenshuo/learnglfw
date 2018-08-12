#include <GLFW/glfw3.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "utils/math.h"
#include "utils/time.h"
//
int global_count;
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    printf("%s__%d\n", "resize", global_count);
    global_count++;
    glViewport(0, 0, width, height);
}

//
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        printf("you input 9\n");
}
//
time_t ts;
void timestamp()
{
    time(&ts);
    printf("timeis:%ld\n", ts);
}

//
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};
//
int main(void)
{
    GLFWwindow *window;
    int *time;
    int *time1;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glViewport(320, 240, 50, 50);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    float r = 0.01;
    float step = 0.01;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // sleep
        usleep(28000);
        // get frame per second
        int fps = getfps();
        if (fps > 0) {
            printf("fps:%d\n", fps);
        }
        // process input
        processInput(window);

        /* Render here */
        r = plus_unit(r, step);

        glClearColor(r, r, r, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
