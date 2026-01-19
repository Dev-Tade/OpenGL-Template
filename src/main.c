#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <gl_debug.h>
#include <shaders.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct {
  GLFWwindow *window;
  uint32_t vao;
  uint32_t vbo;
  uint32_t shader;
} Context;

// Forward Declarations

static void glfw_error_cb(int error, const char *desc);
static void glfw_framebuffer_size_cb(GLFWwindow *window, int width, int height);

void create_buffers(uint32_t *vao, uint32_t *vbo);
void delete_buffers(uint32_t *vao, uint32_t *vbo);
void bind_buffers(uint32_t *vao, uint32_t *vbo);
void unbind_buffers();
void vertex_attributes();
void create_shader(uint32_t *shader);
void check_shader(uint32_t shader, GLboolean link);

#define DATA_FIELDS 7

// X, Y, Z, R, G, B, A
const float triangle_data[DATA_FIELDS * 3] = { 
  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
};

int main(int argc, char **argv) 
{
  Context ctx = {NULL, 0, 0, 0};

  glfwSetErrorCallback(glfw_error_cb);
  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  ctx.window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Template", NULL, NULL);
  if (ctx.window == NULL) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(ctx.window);
  
  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    exit(EXIT_FAILURE);
  
  printf("Loaded OpenGL: %d.%d\n", GLVersion.major, GLVersion.minor);
  opengl_print_info();
  opengl_debug_enable();
  
  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(ctx.window, glfw_framebuffer_size_cb);
  
  create_shader(&ctx.shader);
  
  create_buffers(&ctx.vao, &ctx.vbo);
  glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * DATA_FIELDS * 3), triangle_data, GL_STATIC_DRAW);

  vertex_attributes();
  unbind_buffers();

  while (!glfwWindowShouldClose(ctx.window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    bind_buffers(&ctx.vao, &ctx.vbo);

    glUseProgram(ctx.shader);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    unbind_buffers();

    glfwSwapBuffers(ctx.window);
    glfwPollEvents();
  }

  glDeleteProgram(ctx.shader);
  delete_buffers(&ctx.vao, &ctx.vbo);

  glfwDestroyWindow(ctx.window);
  glfwTerminate();
  return 0;
}

static void glfw_error_cb(int error, const char *desc)
{
  fprintf(stderr, "[ERROR]: GLFW Error %d -> %s\n", error, desc);
}

void glfw_framebuffer_size_cb(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

inline void create_buffers(uint32_t *vao, uint32_t *vbo) {
  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);

  glGenBuffers(1, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
}

inline void delete_buffers(uint32_t *vao, uint32_t *vbo) {
  glDeleteVertexArrays(1, vao);
  glDeleteBuffers(1, vbo);
}

inline void bind_buffers(uint32_t *vao, uint32_t *vbo) {
  glBindVertexArray(*vao);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
}

inline void unbind_buffers() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

inline void vertex_attributes() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, DATA_FIELDS * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, DATA_FIELDS * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
}

inline void check_shader(uint32_t shader, GLboolean link) {
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

inline void create_shader(uint32_t *shader) {
  uint32_t vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertex_shader_src, NULL);
  glCompileShader(vert);
  check_shader(vert, 0);

  uint32_t frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragment_shader_src, NULL);
  glCompileShader(frag);
  check_shader(frag, 0);

  *shader = glCreateProgram();
  glAttachShader(*shader, vert);
  glAttachShader(*shader, frag);
  glLinkProgram(*shader);
  check_shader(*shader, 1);

  glDeleteShader(vert);
  glDeleteShader(frag);
}