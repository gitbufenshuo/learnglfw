#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "gameobject.h"

static ST_Gameobject **all_objects;
static int object_num;
// 所有的 gameobject 必须统一管理
ST_Gameobject *NewGameobject()
{
    ST_Gameobject *_newone = malloc(sizeof(ST_Gameobject));
    memset(_newone, 0, sizeof(ST_Gameobject));
    return _newone;
}
// 所有数据指针，在外界可以不用保存，函数会拷贝
void SetVerticesAndTriangle(ST_Gameobject *gb,
                            int vertices_num,        // 顶点数量
                            float *vertices,         // 顶点
                            int vertex_step,         // 顶点步长
                            int vertex_location_num, // 顶点种类数量
                            int *vertice_length,     // 顶点步长长度
                            int indices_num,         // 三角点数量
                            unsigned int *indices)   // 三角点
{
    if (gb->set != 0)
    {
        free((gb->mesh)->vertex_length);
        free((gb->mesh)->vertices);
        free((gb->mesh)->indices);
        free(gb->mesh);
        gb->set = 1;
    }

    gb->mesh = malloc(sizeof(ST_Mesh));
    memset(gb->mesh, 0, sizeof(ST_Mesh));

    (gb->mesh)->vertex_step = vertex_step;
    (gb->mesh)->vertices = malloc(sizeof(float) * vertices_num);
    (gb->mesh)->vertex_length = malloc(sizeof(int) * vertex_location_num);
    (gb->mesh)->indices = malloc(sizeof(unsigned int) * indices_num);

    memcpy((gb->mesh)->vertices, vertices, sizeof(float) * vertices_num);
    (gb->mesh)->vertices_num = vertices_num;

    memcpy((gb->mesh)->indices, indices, sizeof(int) * indices_num);
    (gb->mesh)->indices_num = indices_num;

    memcpy((gb->mesh)->vertex_length, vertice_length, sizeof(int) * vertex_location_num);
    (gb->mesh)->vertex_location_num = vertex_location_num;
}

void SetMaterial(ST_Gameobject *gb,
                 char *v_shader_path,
                 char *f_shader_path,
                 char *t_image_path)
{
    if (gb->set != 0)
    {
        free((gb->material)->shader);
        free(gb->material);
        gb->set = 1;
    }
    gb->material = malloc(sizeof(ST_Material));
    memset(gb->material, 0, sizeof(ST_Material));
    (gb->material)->shader = malloc(sizeof(ST_Shader));
    memset((gb->material)->shader, 0, sizeof(ST_Shader));

    memcpy((gb->material)->vertex_shader_path, v_shader_path, strlen(v_shader_path));
    memcpy((gb->material)->fragment_shader_path, f_shader_path, strlen(f_shader_path));
    memcpy((gb->material)->texture_path, t_image_path, strlen(t_image_path));
}
void SetUpdate(ST_Gameobject *gb, int kind, void *fn)
{
    if (kind == 0)
    {
        gb->logic_update = fn;
    }
    if (kind == 1)
    {
        gb->render_update = fn;
    }
}
void SetDrawEnable(ST_Gameobject *gb, int isEnable)
{
    if (isEnable == 1)
    {
        gb->draw_enable = 1;
    }
    else
    {
        gb->draw_enable = 0;
    }
}

int RegisterGameobjectToGlobal(ST_Gameobject *gb)
{
    object_num++;
    ST_Gameobject **newPointerList = malloc(sizeof(ST_Gameobject *) * object_num);
    newPointerList[object_num - 1] = gb;
    for (int i = 0; i != object_num - 1; i++)
    {
        newPointerList[i] = all_objects[i];
    }
    all_objects = newPointerList;
    return object_num - 1;
}
void logic_update(ST_Global *global_info)
{
    for (int i = 0; i != object_num; i++)
    {
        all_objects[i]->logic_update(global_info);
    }
}
void render_update(ST_Global *global_info)
{
    for (int i = 0; i != object_num; i++)
    {
        printf("the render_update %s\n", (unsigned char *)all_objects[i]->render_update);
        if (all_objects[i]->render_update != 0)
        {
            all_objects[i]->render_update(global_info);
        }
    }
}
void last_draw(ST_Global *global_info)
{
    for (int i = 0; i != object_num; i++)
    {
        gameobject_draw(all_objects[i]);
    }
}
void global_update(ST_Global *global_info)
{
    printf("global_update_logic_update\n");
    printf("[%d]\n", object_num);
    logic_update(global_info);

    printf("global_update_render_update\n");
    render_update(global_info);

    printf("global_update_last_draw\n");
    last_draw(global_info);
}
void printMesh(ST_Mesh *mesh)
{
    printf("set->%d\n", mesh->set);
    printf("vbo->%d\n", mesh->VBO);
    printf("vao->%d\n", mesh->VAO);
    printf("ebo->%d\n", mesh->EBO);
    printf("ver_num->%d\n", mesh->vertices_num);
    for (int i = 0; i != mesh->vertices_num; i++)
    {
        printf("%f ", mesh->vertices[i]);
    }
    printf("\n");
    printf("ver_step->%d\n", mesh->vertex_step);
    printf("ver_location_num->%d\n", mesh->vertex_location_num);
    for (int i = 0; i != mesh->vertex_location_num; i++)
    {
        printf("%d ", mesh->vertex_length[i]);
    }
    printf("\n");
    printf("indices_num->%d\n", mesh->indices_num);
    for (int i = 0; i != mesh->indices_num; i++)
    {
        printf("%d ", mesh->indices[i]);
    }
    printf("\n");
}
void printAllGameobject(ST_Gameobject *gb)
{
}