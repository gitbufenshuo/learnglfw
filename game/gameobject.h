#include "../mat/mat.h"
#ifdef GAMEOBJECTH
#else
typedef struct
{
    unsigned int ID;
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
    int *vertex_length; // element num should be vertex_location_num
    unsigned int *indices;
    int indices_num;
    Fn_SetAllContext setAll;
} ST_Mesh;

typedef struct
{
    char vertex_shader_path[50];   // vertex shader 文件的路径
    char fragment_shader_path[50]; // fragment shader 文件的路径
    char texture_path[50];         // 纹理图片的路径
    unsigned int TBO;              // texture buffer object
    ST_Shader *shader;
} ST_Material;
typedef struct
{
    ST_VEC3 camera_pos;    // 相机位置
    ST_VEC3 camera_front;  // 相机看的方向
    ST_VEC3 camera_up;     // should be constant 人工选定的世界坐标下的 up
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
    char keyPressed;
    double mouseX;
    double mouseY;
} ST_Global;
typedef void (*Fn_Update)(ST_Global *global_info);
typedef struct
{
    int set;
    float x;
    float y;
    float z;
    float x_rotate_degree;
    float y_rotate_degree;
    float z_rotate_degree;
    int draw_prepared; // 是否准备好绘画
    int draw_enable;   // 失效/使能 绘画
    ST_Material *material;
    ST_Mesh *mesh;
    Fn_Update logic_update;
    Fn_Update render_update;
    void *custom;
} ST_Gameobject;

#define GAMEOBJECTH 00000000
#endif
extern void gameobject_draw(ST_Gameobject *gb);
extern ST_Gameobject *NewGameobject();
extern void SetVerticesAndTriangle(ST_Gameobject *gb,
                                   int vertices_num,        // 顶点数量
                                   float *vertices,         // 顶点
                                   int vertex_step,         // 顶点步长
                                   int vertex_location_num, // 顶点种类数量
                                   int *vertice_length,     // 顶点步长长度
                                   int indices_num,         // 三角点数量
                                   unsigned int *indices);  // 三角点
extern void SetMaterial(ST_Gameobject *gb,
                        char *v_shader_path,
                        char *f_shader_path,
                        char *t_image_path);
extern void SetUpdate(ST_Gameobject *gb, int kind, void *fn);
extern void SetDrawEnable(ST_Gameobject *gb, int isEnable);
extern int RegisterGameobjectToGlobal(ST_Gameobject *gb);
extern void draw_init();
extern void global_update(ST_Global *global_info);
extern ST_Global *return_global_info();
extern void printMesh(ST_Mesh *mesh);