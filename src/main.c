#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <shaders.h>

#define WIDTH 700
#define HEIGHT 600

typedef struct {
  GLFWwindow *window;
  uint32_t vao;
  uint32_t vbo;
  uint32_t shader;
} Context;

// Forward Declarations
void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const GLvoid* user);
void CreateBuffers(uint32_t *vao, uint32_t *vbo);
void DeleteBuffers(uint32_t *vao, uint32_t *vbo);
void BindBuffers(uint32_t *vao, uint32_t *vbo);
void UnbindBuffers();
void VertexAttributes();
void CreateShader(uint32_t *shader);
void CheckShader(uint32_t shader, GLboolean link);

#define DATA_FIELDS 7

// X, Y, Z, R, G, B, A
const float triangle_data[DATA_FIELDS * 3] = { 
  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
};

int main(int argc, char **argv) {
  Context ctx = {NULL, 0, 0, 0};

  if (!glfwInit()) {
    fprintf(stderr, "GLFW::INIT\n");
    return -1;
  }

  glfwWindowHint(GLFW_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);

  ctx.window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Template", NULL, NULL);
  if (!ctx.window) {
    fprintf(stderr, "GLFW::CREATE_WINDOW\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(ctx.window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
    fprintf(stderr, "GLAD::LOAD\n");
    return -1;
  }

#ifdef ENABLE_GL_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(DebugCallback, (void*)0);
#endif

  CreateShader(&ctx.shader);

  CreateBuffers(&ctx.vao, &ctx.vbo);
  glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * DATA_FIELDS * 3), triangle_data, GL_STATIC_DRAW);

  VertexAttributes();
  UnbindBuffers();


  while (!glfwWindowShouldClose(ctx.window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    BindBuffers(&ctx.vao, &ctx.vbo);

    glUseProgram(ctx.shader);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    UnbindBuffers();

    glfwSwapBuffers(ctx.window);
    glfwPollEvents();
  }

  glDeleteProgram(ctx.shader);
  DeleteBuffers(&ctx.vao, &ctx.vbo);

  glfwDestroyWindow(ctx.window);
  glfwTerminate();
  return 0;
}

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const GLvoid* user) {
  fprintf(stderr, 
    "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, msg
  );
}

inline void CreateBuffers(uint32_t *vao, uint32_t *vbo) {
  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);

  glGenBuffers(1, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
}

inline void DeleteBuffers(uint32_t *vao, uint32_t *vbo) {
  glDeleteVertexArrays(1, vao);
  glDeleteBuffers(1, vbo);
}

inline void BindBuffers(uint32_t *vao, uint32_t *vbo) {
  glBindVertexArray(*vao);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
}

inline void UnbindBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

inline void VertexAttributes() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, DATA_FIELDS * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, DATA_FIELDS * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
}

inline void CheckShader(uint32_t shader, GLboolean link) {
  int32_t success = 0;
  char *message = 0;
  char error[512] = {0};

  if (link) {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(shader, 512, NULL, error);
    message = "Program Linking Failed";
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(shader, 512, NULL, error);
    message = "Shader Compilation Failed";
  }

  if (!success) fprintf(stderr, "%s =>\n\t%s\n", message, error);
}

inline void CreateShader(uint32_t *shader) {
  uint32_t vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &VertexShaderSrc, NULL);
  glCompileShader(vert);
  CheckShader(vert, 0);

  uint32_t frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &FragmentShaderSrc, NULL);
  glCompileShader(frag);
  CheckShader(frag, 0);

  *shader = glCreateProgram();
  glAttachShader(*shader, vert);
  glAttachShader(*shader, frag);
  glLinkProgram(*shader);
  CheckShader(*shader, 1);

  glDeleteShader(vert);
  glDeleteShader(frag);
}