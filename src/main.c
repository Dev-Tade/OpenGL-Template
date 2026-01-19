#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <gl_debug.h>
#include <shaders.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Template"

typedef struct {
  GLFWwindow *window;
  uint32_t vao;
  uint32_t vbo;
  uint32_t shader;
} Context;

// Forward Declarations

static void glfw_error_cb(int error, const char *desc);
static void glfw_framebuffer_size_cb(GLFWwindow *window, int width, int height);

static inline void create_buffers(uint32_t *vao, uint32_t *vbo);
static inline void delete_buffers(uint32_t *vao, uint32_t *vbo);
static inline void bind_buffers(uint32_t vao, uint32_t vbo);
static inline void unbind_buffers(void);
static inline void setup_vertex_attrs(void);
static inline bool create_shader(uint32_t *shader);

// Vertex Data

#define VERTEX_COMPONENTS 7

const float triangle_data[VERTEX_COMPONENTS * 3] = 
{ 
//  X      Y     Z       R     G     B     A
  -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
   0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
};

int main(int argc, char **argv) 
{
  Context ctx = {NULL, 0, 0, 0};

  glfwSetErrorCallback(glfw_error_cb);
  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  ctx.window = glfwCreateWindow(
    WINDOW_WIDTH, WINDOW_HEIGHT,
    WINDOW_TITLE,
    NULL, NULL
  );

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
  
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(ctx.window, glfw_framebuffer_size_cb);
  
  if (!create_shader(&ctx.shader)) {
    fprintf(stderr, "[ERROR]: Shader creation failed\n");
    exit(EXIT_FAILURE);
  }
  
  create_buffers(&ctx.vao, &ctx.vbo);
  bind_buffers(ctx.vao, ctx.vbo);

  glBufferData(
    GL_ARRAY_BUFFER, 
    VERTEX_COMPONENTS * 3 * sizeof(float),
    triangle_data,
    GL_STATIC_DRAW
  );

  setup_vertex_attrs();
  unbind_buffers();

  while (!glfwWindowShouldClose(ctx.window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    bind_buffers(ctx.vao, ctx.vbo);

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

static inline void create_buffers(uint32_t *vao, uint32_t *vbo)
{
  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);
}

static inline void delete_buffers(uint32_t *vao, uint32_t *vbo)
{
  glDeleteVertexArrays(1, vao);
  glDeleteBuffers(1, vbo);
}

static inline void bind_buffers(uint32_t vao, uint32_t vbo)
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

inline void unbind_buffers(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static inline void setup_vertex_attrs(void) 
{
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_TRUE, 
    VERTEX_COMPONENTS * sizeof(float),
    (void*)0
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 4, GL_FLOAT, GL_TRUE,
    VERTEX_COMPONENTS * sizeof(float),
    (void*)(3 * sizeof(float))
  );
  glEnableVertexAttribArray(1);
}

static inline bool shader_ok(uint32_t shader) 
{
  int32_t status = 0;
  char diagnostic[512] = {0};

  if (glIsProgram(shader)) {
    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(shader, 512, NULL, diagnostic);
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(shader, 512, NULL, diagnostic);
  }

  if (status == 0) {
    fprintf(
      stderr, "[ERROR]: %s =>\n\t%s\n",
      glIsProgram(shader) ? 
        "Program linking failed" :
        "Shader compilation failed",
      diagnostic
    );

    return false;
  }

  return true;
}

static inline bool create_shader(uint32_t *shader) 
{
  uint32_t vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertex_shader_src, NULL);
  glCompileShader(vert);
  if (!shader_ok(vert)) return false;

  uint32_t frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragment_shader_src, NULL);
  glCompileShader(frag);
  if (!shader_ok(frag)) return false;

  *shader = glCreateProgram();
  glAttachShader(*shader, vert);
  glAttachShader(*shader, frag);
  glLinkProgram(*shader);
  if (!shader_ok(*shader)) return false;

  glDeleteShader(vert);
  glDeleteShader(frag);

  return true;
}