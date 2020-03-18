#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtx/rotate_vector.hpp>
#include "src/vpe/euclideanSpace.h"
#include "src/vpe/shaderConfig.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

vpe::shaderConfig config;
vpe::euclideanSpace eSpace;

glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, -200.0f);
glm::vec3 CAMERA_DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

void update_camera(){
    glUniform3f(glGetUniformLocation(config.getProgram(), "CAMERA_POSITION"), CAMERA_POSITION.x, CAMERA_POSITION.y, CAMERA_POSITION.z);
    glUniform3f(glGetUniformLocation(config.getProgram(), "CAMERA_DIRECTION"), CAMERA_DIRECTION.x, CAMERA_DIRECTION.y, CAMERA_DIRECTION.z);
    glUniform3f(glGetUniformLocation(config.getProgram(), "CAMERA_UP"), CAMERA_UP.x, CAMERA_UP.y, CAMERA_UP.z);
}

void update_spheres(){
    for(int i = 0; i < eSpace.getObjectCount(); i++){
        vpe::sphere obj = eSpace.getObject(i);
        glUniform3f(obj.id, obj.position.x, obj.position.y, obj.position.z);
    }
}

void render(){
    eSpace.run();
    update_spheres();
    glClear(GL_COLOR_BUFFER_BIT);
    glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
    glFlush();
    glutPostRedisplay();
}

void key_press(unsigned char key, int x, int y){
    switch(key){
        case 'w' :
        CAMERA_POSITION += CAMERA_DIRECTION;
        break;
        case 'a' :
        CAMERA_DIRECTION = glm::rotate(CAMERA_DIRECTION, -0.1f, CAMERA_UP);
        break;
        case 's' :
        CAMERA_POSITION -= CAMERA_DIRECTION;
        break;
        case 'd' :
        CAMERA_DIRECTION = glm::rotate(CAMERA_DIRECTION, 0.1f, CAMERA_UP);
        break;
        case 'q' :
        CAMERA_DIRECTION = glm::rotate(CAMERA_DIRECTION, -0.1f, glm::cross(CAMERA_UP, CAMERA_DIRECTION));
        CAMERA_UP = glm::rotate(CAMERA_UP, -0.1f, glm::cross(CAMERA_UP, CAMERA_DIRECTION));
        break;
        case 'e' :
        CAMERA_DIRECTION = glm::rotate(CAMERA_DIRECTION, 0.1f, glm::cross(CAMERA_UP, CAMERA_DIRECTION));
        CAMERA_UP = glm::rotate(CAMERA_UP, 0.1f, glm::cross(CAMERA_UP, CAMERA_DIRECTION));
        break;
    }
    
    update_camera();
    update_spheres();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
    glutCreateWindow("opengl project");
    glutDisplayFunc(render);
    glutKeyboardFunc(key_press);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW tanımlaması esnasında hata olustu:" << glewGetErrorString(0) << std::endl;
    }
    else {
        std::cout << "GLEW versiyon: " << glewGetString(GLEW_VERSION) << std::endl;
    }
    
    config.shaderExtensionControl();
    config.init();
    config.readShaderSource("vertex.shader", GL_VERTEX_SHADER);
    config.readShaderSource("fragment.shader", GL_FRAGMENT_SHADER);
    
    config.compileShader(GL_VERTEX_SHADER);
    config.compileShader(GL_FRAGMENT_SHADER);
    
    config.attachShader(GL_VERTEX_SHADER);
    config.attachShader(GL_FRAGMENT_SHADER);
    
    config.linkProgram();
    config.useProgram();
    
    eSpace.addObject(glGetUniformLocation(config.getProgram(), "SPHERES[0].center"), 120000.0f, 120.0f, glm::vec3(0, 0, 180.0f));
    eSpace.addObject(glGetUniformLocation(config.getProgram(), "SPHERES[1].center"), 1.0f, 4.0f, glm::vec3(40.0f, -10, 50.0f), glm::vec3(1.0f, 0, 0));
    eSpace.addObject(glGetUniformLocation(config.getProgram(), "SPHERES[2].center"), 1.0f, 2.0f, glm::vec3(30.0f, 20.0, 50.0f), glm::vec3(1.0f, 0, 0));

    glUniform1f(glGetUniformLocation(config.getProgram(), "TRANSFORM_X"), 16.0f / (SCREEN_WIDTH - 1));
    glUniform1f(glGetUniformLocation(config.getProgram(), "TRANSFORM_Y"), 9.0f / (SCREEN_HEIGHT - 1));
    update_camera();
    glutReshapeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutMainLoop();
	return 0;
}
