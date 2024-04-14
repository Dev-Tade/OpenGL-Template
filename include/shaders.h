#ifndef SHADERS_H
#define SHADERS_H

const char *vertex_shader_src = 
  "#version 330 core\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 proj;\n"
  "layout (location = 0) in vec3 Position;\n"
  "void main(void) {\n"
  "  gl_Position = proj * view * model * vec4(Position, 1.0f);\n"
  "}\0"
;

const char *fragment_shader_src = 
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main(void) {\n"
  " FragColor = vec4(1.0f, 0.7f, 0.3f, 1.0f);\n"
  "}\0"
;

#endif