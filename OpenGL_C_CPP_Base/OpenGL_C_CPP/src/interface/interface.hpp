#ifndef INTERFACE_H 
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "io/io.hpp"

class Interface {
private:
    struct {
        GLFWwindow* instance;
        GLint width;
        GLint height;
    } window;

    struct {
        GLchar log[512];
        GLuint program;
    } opengl;

public:
    struct {
        clock_t elapsedTime;
    } state;
    int start();
    int refresh(float*);
    int stop();
};

#endif