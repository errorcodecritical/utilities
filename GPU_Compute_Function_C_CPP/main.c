#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>

int init() {

    if (!glfwInit()) { return -1; }

    GLFWwindow* window = glfwCreateWindow(1, 1, "gl_application_context", NULL, NULL);

    if (!window) { return -1; }

    glfwMakeContextCurrent(window);

    return 0;
}

int getFileData(const char* path, char* buffer[], int* length) {
    FILE* file = fopen(path, "r");
    if (!file) { return -1; }

    fseek(file, 0L, SEEK_END);
    *length = ftell(file) * sizeof(char);
    fseek(file, 0L, SEEK_SET);
    *buffer = malloc(*length);

    fread(*buffer, 1, *length, file);
    fclose(file);

    return 0;
}

GLchar glErrorLog[512] = {0};
unsigned int load(const char* path) {
    char* source;
    int length;

    GLuint program = glCreateProgram();
    GLuint compute = glCreateShader(GL_COMPUTE_SHADER);

    if (getFileData(path, &source, &length)) {
        return 0; // get was unsuccessful, abort
    }
    
    glShaderSource(compute, 1, (const GLchar* const*) &source, (const GLint*) &length);
    glCompileShader(compute); 
    glAttachShader(program, compute);
    glLinkProgram(program);
    glDeleteShader(compute);
    free(source);

    glGetShaderInfoLog(compute, 512, NULL, glErrorLog);
    if (*glErrorLog) {
        printf("LOG: %s\n", glErrorLog);
    }

    glGetProgramInfoLog(program, 512, NULL, glErrorLog);
    if (*glErrorLog) {
        printf("LOG: %s\n", glErrorLog);
    }
    
    return program;
}

void* call(unsigned int program, int function_id, int nthreads, void* argv, int args) {
    printf("Called function: %d\n", function_id);
    int resultBufferSize = nthreads * sizeof(unsigned int);

    // initialize input buffer
    GLuint inputBufferHandle;
    glGenBuffers(1, &inputBufferHandle);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBufferHandle);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBufferHandle);
    glBufferData(GL_SHADER_STORAGE_BUFFER, args, argv, GL_DYNAMIC_COPY);

    // initialize output buffer
    GLuint outputBufferHandle;
    glGenBuffers(1, &outputBufferHandle);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBufferHandle);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBufferHandle);
    glBufferData(GL_SHADER_STORAGE_BUFFER, resultBufferSize, NULL, GL_DYNAMIC_COPY);
    
    // run compute
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "function_id"), function_id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBufferHandle);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBufferHandle);
    glDispatchCompute(nthreads, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // fetch result
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBufferHandle);
    void* result = memcpy(
        malloc(nthreads * sizeof(int)), 
        glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, resultBufferSize, GL_MAP_READ_BIT), 
        resultBufferSize
    );
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return result;
}

// example: create a compute program with functions that work on and return a 100 element array of ints;

int main() {
    init(); // for creating gl context, replace with your own implementation

    unsigned int program = load("./function.comp");
    
    int baseValue = 22;
    int* result1 = call(program, 0, 100, &baseValue, 1 * sizeof(int));
    int* result2 = call(program, 1, 100, result1, 100 * sizeof(int));
    
    for (int i = 0; i < 100; i++) {
        printf("%d, ", result1[i]);
    }

    printf("\n\n");

    for (int i = 0; i < 100; i++) {
        printf("%d, ", result2[i]);
    }

    printf("\n");
    
    free(result2);
    free(result1);

    return 0;
}
