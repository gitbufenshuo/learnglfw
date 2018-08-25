extern char* fileReadAll (char* filename);
#ifdef UTILS
#else
typedef int AXIS;
typedef struct
{
    float* element;
} ST_MAT4;

typedef struct
{
    float element[4];
} ST_VEC4;
typedef struct
{
    float element[3];
} ST_VEC3;
#endif
extern ST_VEC4* MatVec4(ST_MAT4* mat4, ST_VEC4* vec4);
extern ST_MAT4* NewMat4(int kind);
extern void PrintMat4(ST_MAT4* mat4);
extern ST_MAT4 *MatMat4(ST_MAT4 *mat4_left, ST_MAT4 *mat4_right);
extern ST_MAT4 *D3_Rotate(ST_MAT4 *mat4, float x_degree, float y_degree, float z_degree);
extern ST_MAT4 *D3_Translate(ST_MAT4 *mat4, float x_value, float y_value, float z_value);
extern void SetMat4Identity(ST_MAT4 *mat4);
extern void Mat4SetValue(ST_MAT4 *mat4, int row, int column, float value);
extern void MatVec4_Inplace(ST_MAT4 *mat4, ST_VEC4 *vec4);
extern ST_MAT4 *D3_Homoz(ST_MAT4 *mat4, float z);
extern ST_MAT4 *D3_Scale(ST_MAT4 *mat4, float x, float y, float z);
void Mat4Free(ST_MAT4 *mat4);
extern ST_MAT4 *D3_LookAtFrom(ST_VEC3 *point, ST_VEC3 *target, ST_VEC3 *up);

extern void Vec3Free(ST_VEC3 *vec3);
extern ST_VEC3 *NewVec3(float a, float b, float c);
extern ST_VEC3 *Vec3Cross(ST_VEC3 *left, ST_VEC3 *right);
extern void ST_VEC3_InPlace_Normalize(ST_VEC3 *vec3);
extern ST_VEC3 *ST_VEC3_Sub(ST_VEC3 *front, ST_VEC3 *back);
extern ST_VEC3 *ST_VEC3_Add(ST_VEC3 *front, ST_VEC3 *back);
extern void PrintVec3(ST_VEC3 *vec3);

extern void PrintVec4(ST_VEC4 *vec4);

extern float RadiusOfDegree(float);



