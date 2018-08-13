#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertex_shader = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nlayout (location = 1) in float xScale;\nvoid main()\n{\n    gl_Position = vec4(aPos.x * xScale, aPos.y, aPos.z, 1.0);\n}\n";
const char *fragment_shader = "#version 330 core\nout vec4 FragColor;\n\nvoid main()\n{\n    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n} \n";
float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.8f,
    0.5f, -0.5f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.8f,
    0.0f, 0.0f, 0.0f, 0.8f,
    0.0f, 0.9f, 0.0f, 0.8f,
    0.9f, 0.0f, 0.0f, 0.8f};
unsigned int shaderProgram;
void compile()
{
    // vertex
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    // fragment
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
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



    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 16 * 4, vertices, GL_STATIC_DRAW);
    // 3.0 bind VAO should be here before set vertex attribute
    glBindVertexArray(VAO[0]);
    // 3.1 then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(  0 ) );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(  12 ) );
    glEnableVertexAttribArray(1);


    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 12 * 4, vertices+16, GL_STATIC_DRAW);
    // 3.0 bind VAO should be here before set vertex attribute
    glBindVertexArray(VAO[1]);
    // 3.1 then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(  0 ) );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(  12 ) );
    glEnableVertexAttribArray(1);
    // last. config is set, you can do render in the main loop

}
int jiou = 0;
void draw_triangle()
{
    jiou++;
    prepare_draw_triangle();
    glBindVertexArray(VAO[jiou%2]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
