#ifndef SHADERCONFIG_H
#define SHADERCONFIG_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
namespace vpe
{

class shaderConfig
{
    GLuint vertexShader, fragmentShader;
    GLint program;
public:
    shaderConfig();
    ~shaderConfig();
    void init();
    void shaderExtensionControl();
    void readShaderSource(const char*, GLint);
    void compileShader(GLint);
    void attachShader(GLint);
    void linkProgram();
    void useProgram();
    GLint getProgram();
};

}

#endif // SHADERCONFIG_H
