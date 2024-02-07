#ifndef SHADERS_H
#define SHADERS_H

const char *vertex_shader_src = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec4 aClr;\n"
  "out vec4 outColor;\n"
  "void main(void) {\n"
  "  gl_Position = vec4(aPos, 1.0f);\n"
  "  outColor = aClr;\n"
  "}\0"
;

const char *fragment_shader_src = 
  "#version 330 core\n"
  "in vec4 outColor;\n"
  "out vec4 FragColor;\n"
  "void main(void) {\n"
  "FragColor = outColor;\n"
  "}\0"
;

#endif