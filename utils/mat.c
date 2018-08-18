#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void PrintMat4(ST_MAT4 *mat4)
{
    printf("-\n");
    float *mat4ele = mat4->element;
    for (int i = 0; i != 4; i++)
    {
        printf("%f %f %f %f\n", mat4ele[i * 4], mat4ele[i * 4 + 1], mat4ele[i * 4 + 2], mat4ele[i * 4 + 3]);
    }
}