#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gameobject.h"
#include "../mat/mat.h"

ST_Global global_info;

ST_MAT4 *model(ST_Gameobject *gb)
{
    ST_MAT4 *transform = NewMat4(0);
    ST_MAT4 *old = transform;
    transform = D3_Rotate(transform, gb->x_rotate_degree, gb->y_rotate_degree, gb->z_rotate_degree);
    Mat4Free(old);
    // second : translate
    old = transform;
    transform = D3_Translate(transform, gb->x, gb->y, gb->z);
    Mat4Free(old);
    return transform;
}
ST_MAT4 *view(ST_Gameobject *gb) // lots about camera, so we put camera into global_info
{
    ST_CUS_CAMERA *camera_object = global_info.camera;
    ST_VEC3_Add_InplaceOP(&(camera_object->camera_pos), &(camera_object->camera_front), &(camera_object->camera_target));
    ST_MAT4 *viewT = D3_LookAtFrom(&(camera_object->camera_pos), &(camera_object->camera_target), &(camera_object->camera_up));
    return viewT;
}
ST_MAT4 *project(ST_Gameobject *gb)
{
    ST_CUS_CAMERA *camera_object = global_info.camera;

    ST_MAT4 *transform = NewMat4(0);
    ST_MAT4 *old = transform;
    transform = D3_Homoz(transform, camera_object->near_distance);
    Mat4Free(old);
    return transform;
}
ST_MAT4 *MVP(ST_Gameobject *gb)
{
    ST_MAT4 *M = model(gb);
    ST_MAT4 *V = view(gb);
    ST_MAT4 *P = project(gb);
    ST_MAT4 *VM = MatMat4(V, M);
    ST_MAT4 *PVM = MatMat4(P, VM);
    free(M);
    free(V);
    free(P);
    free(VM);
    return PVM;
}

void change_VAO(ST_Gameobject *gb)
{
    ST_Mesh *my_mesh = gb->mesh;
    glBindVertexArray(my_mesh->VAO);
}
void change_VBO(ST_Gameobject *gb)
{
    ST_Mesh *my_mesh = gb->mesh;
    glBindBuffer(GL_ARRAY_BUFFER, my_mesh->VBO);
}
void change_EBO(ST_Gameobject *gb)
{
    ST_Mesh *my_mesh = gb->mesh;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_mesh->EBO);
}
void do_draw(ST_Gameobject *gb, ST_MAT4 *mvp){
    change_VAO(gb);
    ST_Shader *myShader = (gb->material)->shader;
    myShader->use((void *)myShader);
    myShader->setMat4((void *)myShader, "transform", mvp->element);
    ST_Mesh *my_mesh = gb->mesh;
    glDrawElements(GL_TRIANGLES, my_mesh->indices_num, GL_UNSIGNED_INT, 0);
}
void init_for_draw(ST_Gameobject *gb){
    ST_Mesh *my_mesh = gb->mesh;
    glGenBuffers(1, &(my_mesh->VBO));
    glGenVertexArrays(1, &(my_mesh->VAO));
    glGenBuffers(1, &(my_mesh->EBO));
    change_EBO(gb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (my_mesh->vertices_num), my_mesh->vertices, GL_STATIC_DRAW);
    change_VAO(gb);
    // 3.1 then set our vertex attributes pointers
    for (int location = 0; location != my_mesh->vertex_location_num; location ++)
    {
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * 4));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * 4));
    glEnableVertexAttribArray(2);

}
void gameobject_draw(ST_Gameobject *gb)
{
    if (gb->draw_enable == 0)
    {
        return;
    }
    if (gb->draw_prepared == 0)
    {
        // init something about draw
        init_for_draw(gb);
        gb->draw_prepared = 1;
    }
    // calculate the mvp
    ST_MAT4 *mvp = MVP(gb);
}