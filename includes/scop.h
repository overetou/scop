
#ifndef SCOP_H
# define SCOP_H

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <math.h>
# include <stdio.h>

# define UINT unsigned int
# define WINDOW_HEIGHT 800
# define WINDOW_WIDTH 800
# define USAGE "\nUsage:\t./scop <file>.obj <file>.bmp\n\tOr\n\t./scop [specs"\
" / vertex_shader_source / fragment_shader_source]"
# define VERTEX_SHADER_SOURCE "#version 400\n"\
"layout(location=0) in vec4 in_Position;\n"\
"layout(location=1) in vec4 in_Color;\n"\
"out vec4 ex_Color;\n"\
"void main(void)\n"\
"{\n"\
"	gl_Position = in_Position;\n"\
"	ex_Color = in_Color;\n"\
"}"
# define FRAGMENT_SHADER_SOURCE "#version 400\n"\
"in vec4 ex_Color;\n"\
"out vec4 out_Color;\n"\
"void main(void)\n"\
"{\n"\
"	out_Color = ex_Color;\n"\
"}"

void	process_args(const int argc, const char *argv[]);
void	error_check(char val, const char *msg);
void	create_vbo(UINT *vao_id, UINT *vbo_id, UINT *color_buffer_id);
void	destroy_vbo(UINT *vao_id, UINT *vbo_id, UINT *color_buffer_id);
void	create_shaders(UINT *vert_shad_id, UINT *frag_shad_id, UINT *prog_id);
void	destroy_shaders(UINT *vert_shad_id, UINT *frag_shad_id, UINT *prog_id);

#endif
