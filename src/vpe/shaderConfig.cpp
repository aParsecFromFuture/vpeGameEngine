#include "shaderConfig.h"

vpe::shaderConfig::shaderConfig()
{
}

vpe::shaderConfig::~shaderConfig()
{
}

void vpe::shaderConfig::init(){
    program = glCreateProgram();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  
}

void vpe::shaderConfig::shaderExtensionControl(){
    if (glewGetExtension((const char*) "GL_ARB_vertex_shader") != GL_TRUE){
        std::cout << "GL_ARB_vertex_shader extension is not available!" << std::endl;
    }
    
    if (glewGetExtension((const char*) "GL_ARB_fragment_shader") != GL_TRUE){
        std::cout << "GL_ARB_fragment_shader extension is not available!" << std::endl;
    }
}

void vpe::shaderConfig::readShaderSource(const char *fileName, GLint type){
    std::ifstream file(fileName, std::ios::in);
    int fileLength;
    char *shaderSource;
    
    if(file.is_open()){
        file.seekg(0, std::ios::end);
        fileLength = file.tellg();
        file.seekg(0, std::ios::beg);
        shaderSource = new char[fileLength];
        for(int i = 0; i < fileLength; i++){
            shaderSource[i] = file.get();
        }
        file.close();
        
        switch(type){
            case GL_VERTEX_SHADER :
                glShaderSourceARB(vertexShader, 1, (const char**)&shaderSource, &fileLength);
            break;
            case GL_FRAGMENT_SHADER :
                glShaderSourceARB(fragmentShader, 1, (const char**)&shaderSource, &fileLength);
            break;
        }
        return;
    }
    std::cout << "File can't open" << std::endl;
}

void vpe::shaderConfig::compileShader(GLint type){
    GLint compiled;
    switch(type){
        case GL_VERTEX_SHADER :
            glCompileShaderARB(vertexShader);
            
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                std::cout << "Vertex Shader derleme hatasi.\n" << std::endl;
            }
        break;
        case GL_FRAGMENT_SHADER :
            glCompileShaderARB(fragmentShader);
            
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS,&compiled);
            if (!compiled) {
                std::cout << "Fragment shader derleme hatası.\n" << std::endl;
            }
        break;
    }
}

void vpe::shaderConfig::attachShader(GLint type){
    switch(type){
        case GL_VERTEX_SHADER :
            glAttachShader(program, vertexShader);
        break;
        case GL_FRAGMENT_SHADER :
            glAttachShader(program, fragmentShader);
        break;
    }
}

void vpe::shaderConfig::linkProgram(){
    GLint linked;
    glLinkProgram(program); 
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked){
        std::cout << "Shader baglama hatası.\n" << std::endl;
    }
}

void vpe::shaderConfig::useProgram(){
    glUseProgram(program);
}

GLint vpe::shaderConfig::getProgram(){
    return program;
}