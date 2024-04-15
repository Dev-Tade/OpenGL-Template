#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shaders.h>

#define WIDTH 800
#define HEIGHT 600

#define MAX_TRIANGLES 2048
#define MAX_VERTICES MAX_TRIANGLES * 3

typedef struct {
  float x;
  float y;
} Vec2d;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} Vec4d;

typedef float Mat4[16];

typedef struct {
  Vec2d position;
  Vec4d color;
} Vertex;

typedef struct {
  Vertex triangle_data[MAX_VERTICES];
  
  uint32_t vao;
  uint32_t vbo;
  uint32_t shader;
  
  uint32_t triangle_count;
  Mat4 projection_matrix;
} Renderer;

typedef struct {
  GLFWwindow *window;
  Renderer *renderer;
} Context;

// Forward Declarations

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const GLvoid* user);
void create_shader(uint32_t *shader);
void check_shader(uint32_t shader, GLboolean link);
void ortho(float l, float r, float t, float b, float zn, float zf, Mat4 out);

Renderer *renderer_allocate();
void renderer_deallocate(Renderer *renderer);

void renderer_bind_buffers(Renderer *r);
void renderer_unbind_buffers(Renderer *r);

void renderer_setup(Renderer *r);
void renderer_cleanup(Renderer *r);

void renderer_begin_frame(Renderer *r);
void renderer_end_frame(Renderer *r);

void renderer_push_triangle(Renderer *r, Vec2d coords[3], Vec4d colors[3]);
void renderer_push_quad(Renderer *r, Vec2d coords[4], Vec4d colors[4]);

Vec2d triangle_vertices[3] = {
  {0.0f, 0.0f},
  {0.0f, 50.0f},
  {50.0f, 0.0f},
};

Vec4d triangle_colors[3] = {
  {1.0f, 0.0f, 0.0f, 1.0f},
  {0.0f, 0.0f, 1.0f, 1.0f},
  {0.0f, 1.0f, 0.0f, 1.0f},
};

/*
  t1 = {v1, v2, v3}
  t2 = {v4, v2, v3}

  v1 = x, y | v2 = x + w, y | v3 = x, y + h | v4 = x + w, y + h
*/
Vec2d quad_vertices[4] = {
  {150.0f, 150.0f},
  {350.0f, 150.0f},
  {150.0f, 350.0f},
  {350.0f, 350.0f},
};

Vec4d quad_colors[4] = {
  {1.0f, 0.0f, 0.0f, 1.0f},
  {0.0f, 1.0f, 0.0f, 1.0f},
  {0.0f, 0.0f, 1.0f, 1.0f},
  {1.0f, 0.0f, 0.0f, 1.0f},
};

int main(int argc, char **argv) {
  Context ctx = {NULL, NULL};

  if (!glfwInit()) {
    fprintf(stderr, "GLFW::INIT\n");
    return -1;
  }

  glfwWindowHint(GLFW_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);

  ctx.window = glfwCreateWindow(WIDTH, HEIGHT, "Renderer 2D", NULL, NULL);
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

  ctx.renderer = renderer_allocate();
  if (!ctx.window) {
    fprintf(stderr, "RENDERER::ALLOCATE\n");
    glfwTerminate();
    return -1;
  }

  glfwSetWindowUserPointer(ctx.window, &ctx);
  glfwSetWindowSizeCallback(ctx.window, framebuffer_size_callback);

  glViewport(0, 0, WIDTH, HEIGHT);
  ortho(0, WIDTH, HEIGHT, 0, -1, 1, ctx.renderer->projection_matrix);

#ifdef ENABLE_GL_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(debug_callback, (void*)0);
#endif

  renderer_setup(ctx.renderer);

  while (!glfwWindowShouldClose(ctx.window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  
    renderer_begin_frame(ctx.renderer);
      renderer_push_triangle(
        ctx.renderer,
        triangle_vertices,
        triangle_colors
      );

      renderer_push_quad(
        ctx.renderer,
        quad_vertices,
        quad_colors
      );
    renderer_end_frame(ctx.renderer);
    
    glfwSwapBuffers(ctx.window);
    glfwPollEvents();
  }

  renderer_cleanup(ctx.renderer);
  renderer_deallocate(ctx.renderer);

  glfwDestroyWindow(ctx.window);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  Context *g_ctx = (Context *)glfwGetWindowUserPointer(window);

  glViewport(0, 0, width, height);
  ortho(0, width, height, 0, -1, 1, g_ctx->renderer->projection_matrix);
}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const GLvoid* user) {
  fprintf(stderr, 
    "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, msg
  );
}

void check_shader(uint32_t shader, GLboolean link) {
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

void create_shader(uint32_t *shader) {
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

Renderer *renderer_allocate()
{
  Renderer *r = (Renderer *)malloc(sizeof(Renderer));
  memset(r, 0, sizeof(Renderer));

  return r;
}

void renderer_deallocate(Renderer *renderer)
{
  free(renderer);
}

void renderer_begin_frame(Renderer *r)
{
  glClear(GL_COLOR_BUFFER_BIT);
  r->triangle_count = 0;
}

void renderer_end_frame(Renderer *r)
{
  glUseProgram(r->shader);

  uint32_t proj_loc = glGetUniformLocation(r->shader, "projection_matrix");
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &r->projection_matrix[0]);

  renderer_bind_buffers(r);
  glBufferSubData(GL_ARRAY_BUFFER, 0, r->triangle_count * 3 * sizeof(Vertex), r->triangle_data);

  glDrawArrays(GL_TRIANGLES, 0, r->triangle_count * 3);
}

void renderer_bind_buffers(Renderer *r)
{
  glBindVertexArray(r->vao);
  glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
}

void renderer_unbind_buffers(Renderer *r)
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderer_setup(Renderer *r)
{
  glGenVertexArrays(1, &r->vao);
  glGenBuffers(1, &r->vbo);

  renderer_bind_buffers(r);
  glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);   

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);

  create_shader(&r->shader);

  renderer_unbind_buffers(r);

  r->triangle_count = 0;  
}

void renderer_cleanup(Renderer *r)
{
  glDeleteProgram(r->shader);
  glDeleteVertexArrays(1, &r->vao);
  glDeleteBuffers(1, &r->vbo);
}

void renderer_push_triangle(Renderer *r, Vec2d coords[3], Vec4d colors[3])
{
  if (r->triangle_count == MAX_TRIANGLES) {
    renderer_end_frame(r);
    renderer_begin_frame(r);
  }

  for (uint8_t i = 0; i < 3; ++i) {
    r->triangle_data[r->triangle_count * 3 + i].position = coords[i];
    r->triangle_data[r->triangle_count * 3 + i].color = colors[i];
  }

  r->triangle_count += 1;
}

void renderer_push_quad(Renderer *r, Vec2d coords[4], Vec4d colors[4])
{
  // Flush it anyways
  if (r->triangle_count == MAX_TRIANGLES) {
    renderer_end_frame(r);
    renderer_begin_frame(r);
  }

  Vec2d t1v[3] = {coords[0], coords[1], coords[2]};
  Vec2d t2v[3] = {coords[3], coords[1], coords[2]};

  Vec4d t1c[3] = {colors[0], colors[1], colors[2]};
  Vec4d t2c[3] = {colors[3], colors[1], colors[2]};

  renderer_push_triangle(r, t1v, t1c);
  renderer_push_triangle(r, t2v, t2c);
}

void ortho(float l, float r, float b, float t, float zn, float zf, Mat4 out)
{
  if (l == r || t == b || zn == zf)
  {
    fprintf(stderr, "ORTHO::INVALID_ARGUMENTS\n");
    exit(1);
  }

  out[0] = 2.0f / (r - l);
  out[1] = 0.0f;
  out[2] = 0.0f;
  out[3] = 0.0f;

  out[4] = 0.0f;
  out[5] = 2.0f / (t - b);
  out[6] = 0.0f;
  out[7] = 0.0f;

  out[8] = 0.0f;
  out[9] = 0.0f;
  out[10] = -2.0f / (zf - zn);
  out[11] = 0.0f;

  out[12] = -(r + l) / (r - l);
  out[13] = -(t + b) / (t - b);
  out[14] = -(zf + zn) / (zf - zn);
  out[15] = 1.0f;
}