extern void change_smooth(float step);
extern void draw_triangle();

#ifdef HELLO
#else
// ST_Shader struct
typedef void (*Fn_CompileShader)(void *self, char *vs, char *fs);
typedef void (*Fn_UseShaderProgram)(void *self);
typedef void (*Fn_SetShaderInt)(void *self, char *name, int value);
typedef void (*Fn_SetShaderFloat)(void *self, char *name, float value);
typedef struct
{
    unsigned int ID;
    Fn_CompileShader compile;
    Fn_UseShaderProgram use;
    Fn_SetShaderInt setInt;
    Fn_SetShaderFloat setFloat;
} ST_Shader;

typedef struct
{
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    float* vertices;
    int vertices_num;
    unsigned int* indices;

} ST_Mesh;
#define HELLO 1111
#endif