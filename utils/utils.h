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
    float* element;
} ST_VEC4;
typedef struct
{
    float element[3];
} ST_VEC3;
#endif
extern ST_VEC4* MatVec4(ST_MAT4* mat4, ST_VEC4* vec4);
extern ST_MAT4* NewMat4(int kind);
extern ST_VEC4* NewVec4(float one, float two, float three, float four);
extern void PrintMat4(ST_MAT4* mat4);
extern ST_MAT4 *MatMat4(ST_MAT4 *mat4_left, ST_MAT4 *mat4_right);
extern ST_MAT4 *D3_Rotate(ST_MAT4 *mat4, float x_degree, float y_degree, float z_degree);
extern ST_MAT4 *D3_Translate(ST_MAT4 *mat4, float x_value, float y_value, float z_value);
extern void SetMat4Identity(ST_MAT4 *mat4);
extern ST_MAT4 *D3_Homoz(ST_MAT4 *mat4);
extern ST_MAT4 *D3_Scale(ST_MAT4 *mat4, float x, float y, float z);
void Mat4Free(ST_MAT4 *mat4);
extern void Vec3Free(ST_VEC3 *vec3);
extern ST_VEC3 *NewVec3(float a, float b, float c);
extern ST_VEC3 *Vec3Cross(ST_VEC3 *left, ST_VEC3 *right);



