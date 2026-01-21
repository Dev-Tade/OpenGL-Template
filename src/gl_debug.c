
#include <gl_debug.h>
#include <glad/glad.h>

static FILE *debug_output_file = NULL;

#if defined(GL_DEBUG_ENABLED)
#if defined(GL_VERSION_4_3) || (defined(GL_KHR_debug) && GL_KHR_debug == 1)

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
  if (debug_output_file == NULL) debug_output_file = stderr;

  /*
    Since 4.3 KHR_debug is part of core spec, do a runtime check of
    the context being 4.3 or greater to enable it.
    If version is below 4.3 then do runtime check for GL_KHR_extension
    ensuring that driver supports it.
  */
#if defined(GL_VERSION_4_3)
  // Wrapped in preproc guard to ensure runtime symbol is defined
  if (GLAD_GL_VERSION_4_3 == 0) {
    fprintf(debug_output_file,
      "[WARNING]: OpenGL debug expected via OpenGL Core 4.3 spec but runtime version < 4.3\n"
      "           Debug output will not be enabled.\n"
    );
    return;
  }
#elif defined(GL_KHR_debug)
  // Wrapped in preproc guard to ensure runtime symbol is defined
  if (GLAD_GL_KHR_debug == 0) {
    fprintf(debug_output_file, 
      "[WARNING]: GL_KHR_debug extension enabled, but not supported by driver\n"
      "           Debug output will not be enabled\n"
    );
   return;
  }
#endif // !GL_VERSION_4_3 !GL_KHR_debug

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
  if (debug_output_file == NULL) debug_output_file = stderr;

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

#else   // GL_KHR_debug extension missing && GL_VERSION < 4.3

void opengl_debug_enable(void)
{
  if (debug_output_file == NULL) debug_output_file = stderr;
  
  fprintf(debug_output_file, 
    "[WARNING]: GL_DEBUG_ENABLED defined, but GL_KHR_debug extension is missing.\n"
    "           GL_KHR_debug extension is required if OpenGL version < 4.3\n"
    "           Debug output will not be enabled.\n"
  );
} 

#endif  //!GL_VERSION_4_3 !GL_KHR_debug
#else   // Template debug output is disabled

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

  const char *profile = "Legacy (pre-3.2)";

  /*
    GL_CONTEXT_PROFILE_MASK wasn't a thing until OpenGL 3.2
    do not check for it if not defined 
  */
#if defined(GL_CONTEXT_PROFILE_MASK)
  GLint profile_mask = 0;  
  glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile_mask);
  profile = 
    (profile_mask & GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" :
    (profile_mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) ? "Compatibility" :
    "Unknown";
#endif //!GL_CONTEXT_PROFILE_MASK

  printf(
    "[OpenGL Info]:\n"
    "  - Version   : %s\n"
    "  - Vendor    : %s\n"
    "  - Renderer  : %s\n"
    "  - GLSL Ver. : %s\n"
    "  - Profile   : %s\n",
    version  ? (const char *)version  : "NULL",
    vendor   ? (const char *)vendor   : "NULL",
    renderer ? (const char *)renderer : "NULL",
    glsl_ver ? (const char *)glsl_ver : "NULL",
    profile
  );
}

void opengl_debug_set_output(FILE *output_file)
{
  debug_output_file = output_file;
}