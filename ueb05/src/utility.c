#include "utility.h"

#include <stdlib.h>
#include <stdio.h>

void identity(float* m)
{
    m[0] = 1;  m[4] = 0; m[8]  = 0; m[12] = 0;
    m[1] = 0;  m[5] = 1; m[9]  = 0; m[13] = 0;
    m[2] = 0;  m[6] = 0; m[10] = 1; m[14] = 0;
    m[3] = 0;  m[7] = 0; m[11] = 0; m[15] = 1;
}

void frustum(float left, float right, float top, float bottom, float znear, float zfar, float* m)
{
    m[0]  = 2 * znear / (right - left);
    m[1]  = 0.0f;
    m[2]  = 0.0f;
    m[3]  = 0.0f;

    m[4]  = 0.0f;
    m[5]  = 2 * znear / (top - bottom);
    m[6]  = 0.0f;
    m[7]  = 0.0f;

    m[8]  = (right + left) / (right - left);
    m[9]  = (top + bottom) / (top - bottom);
    m[10] = -(zfar + znear) / (zfar - znear);
    m[11] = -1.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = -2 * zfar*znear / (zfar - znear);
    m[15] = 0.0f;
}

void perspective(float fov, float aspect, float znear, float zfar, float* m) {
    float right = znear * (float)(tan(fov * M_PI / 360.0f));
    float left = -right;
    float top = right / aspect;
    float bottom = -top;
    frustum(left, right, top, bottom, znear, zfar, m);
}

void unitCross(float lhsX, float lhsY, float lhsZ, float rhsX, float rhsY, float rhsZ, float* resX, float* resY, float* resZ)
{
    float x = lhsY*rhsZ - lhsZ*rhsY;
    float y = lhsZ*rhsX - lhsX*rhsZ;
    float z = lhsX*rhsY - lhsY*rhsX;

    float length = sqrtf(x*x + y*y + z*z);

    *resX = x / length;
    *resY = y / length;
    *resZ = z / length;
}

void lookAt(float centerX, float centerY, float centerZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ, float* m)
{
    float viewX = targetX - centerX;
    float viewY = targetY - centerY;
    float viewZ = targetZ - centerZ;

    float length = sqrtf(viewX*viewX + viewY*viewY + viewZ*viewZ);

    float fX = viewX / length;
    float fY = viewY / length;
    float fZ = viewZ / length;

    float sX, sY, sZ;
    float uX, uY, uZ;
    unitCross(fX, fY, fZ, upX, upY, upZ, &sX, &sY, &sZ);
    unitCross(sX, sY, sZ, fX, fY, fZ, &uX, &uY, &uZ);

    m[0]  =  sX;
    m[1]  =  uX;
    m[2]  = -fX;
    m[3]  = 0.0f;

    m[4]  =  sY;
    m[5]  =  uY;
    m[6]  = -fY;
    m[7]  = 0.0f;

    m[8]  =  sZ;
    m[9]  =  uZ;
    m[10] = -fZ;
    m[11] = 0.0f;

    m[12]  = -(sX * centerX + sY * centerY + sZ * centerZ);
    m[13] = -(uX * centerX + uY * centerY + uZ * centerZ);
    m[14] =  (fX * centerX + fY * centerY + fZ * centerZ);
    m[15] = 1.0f;
}

char* readfile(const char* filepath) {
	FILE *f;
	fopen_s(&f, filepath, "rb");

    long fsize;
    char* str;

    if (f == NULL) {
        printf("Could not open the file %s.", filepath);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    str = (char*)malloc(fsize + 1);
    int i = fread(str, fsize, 1, f);
    if(i){}
    fclose(f);

    str[fsize] = 0;

    return str;
}

void checkShaderLog(const char* filename, GLuint shader) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 1) {
        GLchar* buffer = (GLchar*)malloc(length);
        glGetShaderInfoLog(shader, length, &length, buffer);

        fprintf(stderr, "Error in Shader %s: %s", filename, buffer);

        free((void*)buffer);
        exit(1);
    }
}

GLuint createShader(GLenum shaderType, const char* filename) {
    GLuint shader = glCreateShader(shaderType);

    const char* source = readfile(filename);
    glShaderSource(shader, 1, &source, NULL);
    free((void*)source);

    glCompileShader(shader);
    checkShaderLog(filename, shader);

    return shader;
}

void checkProgramLog(GLuint program) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    if (length > 1) {
        GLchar* buffer = (GLchar*)malloc(length);
        glGetProgramInfoLog(program, length, &length, buffer);

        fprintf(stderr, "Error in Program: %s", buffer);
        free(buffer);

        exit(1);
    }
}

GLuint createProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
    /* Erstellen der Shader-Objekte */
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderFilename);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

    /* Erstellen des Programms */
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    checkProgramLog(program);

    /* Nach dem Linken des Programms können die Shader-Objekte wieder
     * gelöscht werden. */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
