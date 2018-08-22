#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void PrintVec3(ST_VEC3 *vec3)
{
    printf("[%f %f %f]\n", (vec3->element)[0], (vec3->element)[1], (vec3->element)[2]);
}
// 释放一个vec矢量的内存
void Vec3Free(ST_VEC3 *vec3)
{
    if (vec3 == 0)
    {
        return;
    }
    free(vec3);
}
// 新建一个矢量vec3
ST_VEC3 *NewVec3(float a, float b, float c)
{
    ST_VEC3 *vec3 = malloc(sizeof(ST_VEC3));
    (vec3->element)[0] = a;
    (vec3->element)[1] = b;
    (vec3->element)[2] = c;
    return vec3;
}
// 两个vec3叉乘
ST_VEC3 *Vec3Cross(ST_VEC3 *left, ST_VEC3 *right)
{
    ST_VEC3 *res = NewVec3(0.0f, 0.0f, 0.0f);
    (res->element)[0] = (left->element)[1] * (right->element)[2] - (left->element)[2] * (right->element)[1];
    (res->element)[1] = (left->element)[2] * (right->element)[0] - (left->element)[0] * (right->element)[2];
    (res->element)[2] = (left->element)[0] * (right->element)[1] - (left->element)[1] * (right->element)[0];
    return res;
}
// 归一化一个 vec3
void ST_VEC3_InPlace_Normalize(ST_VEC3 *vec3)
{
    float x = (vec3->element)[0];
    float y = (vec3->element)[1];
    float z = (vec3->element)[2];
    float weight = sqrt(x*x + y*y + z*z);
    if (fabs(weight) < 0.0000001f)
    {
        return;
    }
    for (int i = 0; i < 3; i++) 
    {
        (vec3->element)[i] = (vec3->element)[i]/weight;
    }
}
ST_VEC3 *ST_VEC3_Sub(ST_VEC3 *front, ST_VEC3 *back)
{
    ST_VEC3 *res = NewVec3(0.0f, 0.0f, 0.0f);
    (res->element)[0] = (front->element)[0] - (back->element)[0];
    (res->element)[1] = (front->element)[1] - (back->element)[1];
    (res->element)[2] = (front->element)[2] - (back->element)[2];
    return res;
}
