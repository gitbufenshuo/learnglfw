#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
ST_VEC4 *MatVec4(ST_MAT4 *mat4, ST_VEC4 *vec4)
{
    float *mat4ele = mat4->element;
    float *vec4ele = vec4->element;
    float temp[4] = {};
    for (int i = 0; i != 4; i++)
    {
        temp[i] = mat4ele[i * 4] * vec4ele[0] + mat4ele[i * 4 + 1] * vec4ele[1] + mat4ele[i * 4 + 2] * vec4ele[2] + mat4ele[i * 4 + 3] * vec4ele[3];
    }
    for (int i = 0; i != 4; i++)
    {
        vec4ele[i] = temp[i];
    }
    return vec4;
}
ST_MAT4 *NewMat4(int kind)
{
    ST_MAT4 *mat4 = malloc(sizeof(ST_MAT4));
    memset(mat4, 0, sizeof(ST_MAT4));
    mat4->element = malloc(sizeof(float) * 16);
    memset(mat4->element, 0, sizeof(float) * 16);
    if (kind == 0)
    { // i
        mat4->element[0] = 1.0f;
        mat4->element[5] = 1.0f;
        mat4->element[10] = 1.0f;
        mat4->element[15] = 1.0f;
    }
    return mat4;
}
ST_VEC4 *NewVec4(float one, float two, float three, float four)
{
    ST_VEC4 *vec4 = malloc(sizeof(ST_VEC4));
    memset(vec4, 0, sizeof(ST_VEC4));
    vec4->element = malloc(sizeof(float) * 4);
    vec4->element[0] = one;
    vec4->element[1] = two;
    vec4->element[2] = three;
    vec4->element[3] = four;
    return vec4;
}

float Mat4Row_Column(ST_MAT4 *mat4, int row, int column)
{
    return (mat4->element)[(row - 1) * 4 + (column - 1)];
}
void PrintMat4(ST_MAT4 *mat4)
{
    printf("-\n");

    for (int row = 1; row != 5; row++)
    {
        printf("%f %f %f %f\n", Mat4Row_Column(mat4, row, 1), Mat4Row_Column(mat4, row, 2), Mat4Row_Column(mat4, row, 3), Mat4Row_Column(mat4, row, 4));
    }
}
void Mat4SetValue(ST_MAT4 *mat4, int row, int column, float value)
{
    (mat4->element)[(row - 1) * 4 + (column - 1)] = value;
}
ST_MAT4 *MatMat4(ST_MAT4 *mat4_left, ST_MAT4 *mat4_right)
{
    ST_MAT4 *res = NewMat4(0);
    float final_ele = 0.0f;
    for (int row = 1; row != 5; row++)
    {
        for (int column = 1; column != 5; column++)
        {
            final_ele = 0.0f;
            for (int index = 0; index != 4; index++)
            {
                float f = Mat4Row_Column(mat4_left, row, 1 + index);
                float b = Mat4Row_Column(mat4_right, 1 + index, column);
                final_ele += (f * b);
                // printf("row-->%d  col-->%d  index-->%d::%f*%f=%f\n", row, column, index, f, b, final_ele);
            }
            Mat4SetValue(res, row, column, final_ele);
        }
    }
    return res;
}
float RadiusOfDegree(float degree)
{
    return (3.141592653 * degree) / 180.0f;
}
// x y z means z y x
ST_MAT4 *D3_Rotate(ST_MAT4 *mat4, float x_degree, float y_degree, float z_degree)
{
    ST_MAT4 *res = NewMat4(0);
    float cosy = cos(RadiusOfDegree(y_degree));
    float cosz = cos(RadiusOfDegree(z_degree));
    float sinz = sin(RadiusOfDegree(z_degree));
    float siny = sin(RadiusOfDegree(y_degree));
    float cosx = cos(RadiusOfDegree(x_degree));
    float sinx = sin(RadiusOfDegree(x_degree));
    (res->element)[0] = cosy * cosz;
    (res->element)[1] = -(cosy * sinz);
    (res->element)[2] = siny;

    (res->element)[4] = cosx * sinz + sinx * siny * cosz;
    (res->element)[5] = cosx * cosz - sinx * siny * sinz;
    (res->element)[6] = -(sinx * cosy);

    (res->element)[8] = sinx * sinz - cosx * siny * cosz;
    (res->element)[9] = sinx * cosz + cosx * siny * sinz;
    (res->element)[10] = cosx * cosy;
    return MatMat4(res, mat4);
}