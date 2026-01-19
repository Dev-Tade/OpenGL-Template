#ifndef GL_DEBUG_H
#define GL_DEBUG_H

#include <stdio.h>

void opengl_print_info(void);
void opengl_debug_enable(void);

void opengl_debug_set_output(FILE *file);

#endif //!GL_DEBUG_H