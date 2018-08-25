#ifdef HELLO
#else
// ST_Shader struct
typedef void (*Fn_CompileShader)(void *self, char *vs, char *fs);
typedef void (*Fn_UseShaderProgram)(void *self);
typedef void (*Fn_SetShaderInt)(void *self, char *name, int value);
typedef void (*Fn_SetShaderFloat)(void *self, char *name, float value);
typedef void (*Fn_SetShaderMat4)(void *self, char *name, float *value);
typedef struct
{
    unsigned int ID;
    Fn_CompileShader compile;
    Fn_UseShaderProgram use;
    Fn_SetShaderInt setInt;
    Fn_SetShaderFloat setFloat;
    Fn_SetShaderMat4 setMat4;
} ST_Shader;

typedef void (*Fn_SetAllContext)(void *self, char *image);
typedef struct
{
    int set;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    float *vertices;
    int vertices_num;
    int vertex_step;
    int vertex_location_num;
    int* vertex_length; // element num should be vertex_location_num
    unsigned int *indices;
    int indices_num;
    Fn_SetAllContext setAll;
} ST_Mesh;

typedef struct
{
    char vertex_shader_path[50]; // vertex shader 文件的路径
    char fragment_shader_path[50]; // fragment shader 文件的路径
    char texture_path[50]; // 纹理图片的路径
    unsigned int TBO;      // texture buffer object
    ST_Shader *shader;
} ST_Material;

typedef struct
{
    float x;
    float y;
    float z;
    float x_rotate_degree;
    float y_rotate_degree;
    float z_rotate_degree;
    int draw_prepared; // 是否准备好绘画
    int draw_enable;   // 失效/使能 绘画
    ST_Material* material;
    ST_Mesh* mesh;
    void *custom;
} ST_Gameobject;

typedef struct
{
    ST_VEC3 camera_pos;   // 相机位置
    ST_VEC3 camera_front; // 相机看的方向
    ST_VEC3 camera_up;    // should be constant 人工选定的世界坐标下的 up
    ST_VEC3 camera_target; // 假想的相机盯着的目标
    float near_distance;
    float far_distance;
    float near_long;
    float far_long;
} ST_CUS_CAMERA; // camera is not an ordinary object, so we redefine it

typedef struct
{
    ST_CUS_CAMERA *camera;
    float timeValue;
} ST_Global;

#define HELLO 00000000
#endif