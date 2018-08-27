#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "utils/math.h"
#include "utils/time.h"
#include "game/gameobject.h"
#include "hello/hello.h"
//
int global_count;
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    printf("%s__%d\n", "resize", global_count);
    global_count++;
    glViewport(0, 0, 500, 500);
}

static char keyP;
static double mouse_xpos;
static double mouse_ypos;
//
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    mouse_xpos = xpos;
    mouse_ypos = ypos;
}

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
    window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
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

    // glViewport(0, 0, 500, 500);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    ///////////////////////////
    // gameobject all init
    printf("mybegin\n");
    draw_init();
    printf("mymid\n");
    InitTriangle();
    printf("myend\n");
    ///////////////////////////
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // sleep
        usleep(280000);
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

        //
        ST_Global *globalinfo =  return_global_info();
        // dealwith opengl input
        globalinfo->keyPressed = keyP;
        globalinfo->mouseX = mouse_xpos;
        globalinfo->mouseX = mouse_ypos;
        
        global_update(globalinfo);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
