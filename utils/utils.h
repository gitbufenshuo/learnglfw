extern char* fileReadAll (char* filename);
#ifdef UTILS
#else
typedef struct
{
    float* element;
} ST_MAT4;

typedef struct
{
    float* element;
} ST_VEC4;
#endif
extern ST_VEC4* MatVec4(ST_MAT4* mat4, ST_VEC4* vec4);
extern ST_MAT4* NewMat4(int kind);
extern ST_VEC4* NewVec4(float one, float two, float three, float four);
extern void PrintMat4(ST_MAT4* mat4);