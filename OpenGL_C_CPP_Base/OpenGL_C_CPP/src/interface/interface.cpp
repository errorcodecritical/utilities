#include "interface.hpp"

GLuint newShaderProgram(const char* vertex_source, const char* fragment_source) {
    GLchar log[512];

    GLuint program = glCreateProgram();
    
    char* source;
    int length;

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    ioReadFile(vertex_source, &source, &length);
    glShaderSource(vertex, 1, (const GLchar* const*) &source, (const GLint*) &length);
    glCompileShader(vertex); 

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    ioReadFile(fragment_source, &source, &length);
    glShaderSource(fragment, 1, (const GLchar* const*) &source, (const GLint*) &length);
    glCompileShader(fragment); 

    glGetShaderInfoLog(fragment, 512, NULL, log);
    if (*log) {
        printf("%s\n", log);
    }

    glGetShaderInfoLog(vertex, 512, NULL, log);
    if (*log) {
        printf("%s\n", log);
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

int Interface::start() {
    // Start GLFW
    if (!glfwInit()) {
        return -1;
    }

    if (!(window.instance = glfwCreateWindow(640, 480, "OpenGL Application", NULL, NULL))) {
        return -1;
    }

    glfwMakeContextCurrent(window.instance);
    glfwGetFramebufferSize(window.instance, &window.width, &window.height);

    opengl.program = newShaderProgram("src/shaders/shader1.vert", "src/shaders/shader1.frag");
    
    state.elapsedTime = clock();
    return 0;
}

int Interface::refresh(float* delta) {

    clock_t __time = clock();
    float __delta = ((float)(__time - state.elapsedTime) / CLOCKS_PER_SEC);

    if (__delta >= (1.0 / 60.0)) {
        state.elapsedTime = __time;
        *delta = __delta;

        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(opengl.program);
        glfwGetFramebufferSize(window.instance, &window.width, &window.height);

        glViewport(0, 0, window.width, window.height);
        glRecti(-1, -1, 1, 1);
        
        glfwSwapBuffers(window.instance);
        
        glUniform1f(glGetUniformLocation(opengl.program, "time"), (float)state.elapsedTime / CLOCKS_PER_SEC);
        glUniform1f(glGetUniformLocation(opengl.program, "width"), (float)window.width);
        glUniform1f(glGetUniformLocation(opengl.program, "height"), (float)window.height);
    }

    return !glfwWindowShouldClose(window.instance);
}

int Interface::stop() {
    glfwTerminate();
    return 0;
}