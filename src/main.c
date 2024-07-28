#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <shaders.h>

#include <cglm/cglm.h>

#define WIDTH 800
#define HEIGHT 600

// #define ENABLE_GL_DEBUG

// Forward Declarations

float vertices[] = {
   0.0f,  0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f
};

#define VERTICES_SIZE (sizeof(vertices)/sizeof(vertices[0]))

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const GLvoid* user);
void create_shader(uint32_t *shader);
void check_shader(uint32_t shader, GLboolean link);

int main(int argc, char **argv) {
  GLFWwindow *window;
  if (!glfwInit()) {
    fprintf(stderr, "GLFW::INIT\n");
    exit(1);
  }

  glfwWindowHint(GLFW_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_RESIZABLE, 0);

  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Template", NULL, NULL);
  if (!window) {
    fprintf(stderr, "GLFW::CREATE_WINDOW\n");
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
    fprintf(stderr, "GLAD::LOAD\n");
    exit(1);
  }

  glViewport(0, 0, WIDTH, HEIGHT);

#ifdef ENABLE_GL_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(debug_callback, (void*)0);
#endif

  uint32_t shader;
  create_shader(&shader);

  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);

  glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE * sizeof(float), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // glm::mat4 model = glm::mat4(1.0f);
  mat4 model; glm_mat4_identity(model);
  mat4 view; glm_mat4_identity(view);
  mat4 proj; glm_mat4_identity(proj);

  glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
  glm_perspective(glm_rad(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f, proj);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);
    
    uint32_t model_loc = glGetUniformLocation(shader, "model");
    uint32_t view_loc = glGetUniformLocation(shader, "view");
    uint32_t proj_loc = glGetUniformLocation(shader, "proj");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model[0]);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float*)proj);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shader);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const GLvoid* user) {
  fprintf(stderr, "GL CALLBACK: %s type = %x, severity = %x, msg: %s\n",
   (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
   type, severity, msg);
}

inline void check_shader(uint32_t shader, GLboolean link) {
  int32_t success = 0;
  const char *message = 0;
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

  if (!success) fprintf(stderr, "%s =>\n%s\n", message, error);
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