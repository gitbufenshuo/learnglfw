extern void change_smooth(float step);
extern void draw_triangle(char key_press);

#ifdef HELLO
#else
// ST_Shader struct
typedef void (*Fn_CompileShader)(void *self, char *vs, char *fs);
typedef void (*Fn_UseShaderProgram)(void *self);
typedef void (*Fn_SetShaderInt)(void *self, char *name, int value);
typedef void (*Fn_SetShaderFloat)(void *self, char *name, float value);
typedef void (*Fn_SetShaderMat4)(void *self, char *name, float* value);
typedef struct
{
    unsigned int ID;
    Fn_CompileShader compile;
    Fn_UseShaderProgram use;
    Fn_SetShaderInt setInt;
    Fn_SetShaderFloat setFloat;
    Fn_SetShaderMat4 setMat4;
} ST_Shader;

typedef void (*Fn_SetAllContext)(void *self, char* image);
typedef struct
{
    int set;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int TBO;
    float* vertices;
    int vertices_num;
    unsigned int* indices;
    int indices_num;
    Fn_SetAllContext setAll;
} ST_Mesh;

typedef struct {
    float x;
    float y;
    float z;
    float x_rotate_degree;
    float y_rotate_degree;
    float z_rotate_degree;
    void* custom;
} ST_Gameobject;

typedef struct {
    float front[3];
    float near_distance;
    float far_distance;
    float near_long;
    float far_long;
} ST_CUS_CAMERA;
#define HELLO 1111
#endif