
#include <gl_debug.h>
#include <glad/glad.h>

static FILE *debug_output_file = NULL;

#if defined(GL_DEBUG_ENABLED)
#if defined(GL_KHR_debug) && GL_KHR_debug == 1

static void opengl_debug_message_callback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* msg,
  const GLvoid* user
);

void opengl_debug_enable(void)
{
  if (GLAD_GL_KHR_debug == 0) {
    fprintf(stderr, "[WARNING]: GL_KHR_debug extension present, but not supported by driver\n");
   return;
  }

  if (debug_output_file == NULL) debug_output_file = stderr;

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  glDebugMessageCallback(opengl_debug_message_callback, NULL);

  printf("[INFO]: OpenGL debug messages enabled\n");

  
  glDebugMessageInsert(
    GL_DEBUG_SOURCE_APPLICATION,
    GL_DEBUG_TYPE_MARKER,
    1,
    GL_DEBUG_SEVERITY_NOTIFICATION,
    -1,
    "OpenGL debug messages enabled"
  );
}

static const char *opengl_debug_source(GLenum source)
{
  switch (source) {
    case GL_DEBUG_SOURCE_API:             return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PARTY";
    case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
    case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
  }
  return "Unknown";
}

static const char *opengl_debug_type(GLenum type)
{
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:               return "ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER:              return "MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
    case GL_DEBUG_TYPE_OTHER:               return "OTHER";
  }
  return "Unknown";
}

const char *opengl_debug_severity(GLenum severity)
{
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
    case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
    case GL_DEBUG_SEVERITY_LOW:          return "LOW";
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
  }
  return "Unknown";
}

static void opengl_debug_message_callback(
  GLenum source, GLenum type,
  GLuint id, GLenum severity,
  GLsizei length, const GLchar* msg,
  const GLvoid* user)
{
  fprintf(debug_output_file,
    "[OpenGL Debug]:\n"
    "  - Source   : %s (0x%04X)\n"
    "  - Type     : %s (0x%04X)\n"
    "  - Severity : %s (0x%04X)\n"
    "  - ID       : %u (0x%04X)\n"
    "  - Message  : %s\n",
    opengl_debug_source(source), source,
    opengl_debug_type(type), type,
    opengl_debug_severity(severity), severity,
    id, id,
    msg
  );
}

#else   // GL_KHR_debug -- undefined

void opengl_debug_enable(void)
{
  fprintf(stderr, "[WARNING]: GL_DEBUG_ENABLED defined, but GL_KHR_debug extension is missing\n");
} 

#endif  //!GL_KHR_debug
#else   //GL_DEBUG_ENABLED -- undefined

void opengl_debug_enable(void)
{

}

#endif  //!GL_DEBUG_ENABLED

void opengl_print_info(void)
{
  const GLubyte *version  = glGetString(GL_VERSION);
  const GLubyte *vendor   = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *glsl_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);

  GLint profile_mask = 0;
  glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile_mask);
  const char *profile = 
    (profile_mask & GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" :
    (profile_mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) ? "Compatibility" :
    "Unknown";

  printf("[OpenGL Info]:\n");
  printf("  - Version   : %s\n", version  ? (const char *)version  : "NULL");
  printf("  - Vendor    : %s\n", vendor   ? (const char *)vendor   : "NULL");
  printf("  - Renderer  : %s\n", renderer ? (const char *)renderer : "NULL");
  printf("  - GLSL Ver. : %s\n", glsl_ver ? (const char *)glsl_ver : "NULL");
  printf("  - Profile   : %s\n", profile);
}

void opengl_debug_set_output(FILE *output_file)
{
  debug_output_file = output_file;
}