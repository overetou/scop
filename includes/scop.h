
#ifndef SCOP_H
# define SCOP_H

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <math.h>
# include <stdio.h>
# include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define UINT unsigned int
#define VERTEX_SHADER_SOURCE "#version 440 core\n"\
"layout (location = 0) in vec3 aPos;"\
"out vec4 vertexColor;"\
"void main()"\
"{"\
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"\
"	vertexColor = vec4(0.5, 0, 0, 1);"\
"}"

#define FRAGMENT_SHADER_SOURCE "#version 440 core\n"\
"in vec4 vertexColor;"\
"out vec4 FragColor;"\
"void main()"\
"{"\
"    FragColor = vertexColor;"\
"}"
#define USAGE "Wrong number of arguments.\nUsage:\t./scop"\
" <file>.obj <file>.bmp\n\tOr\n\t./scop [specs /"\
" vertex_shader_source / fragment_shader_source]"

void	process_args(const int argc, const char *argv[]);
void	error_check(char val, const char *msg);
void	error_check_gl(GLenum error);
void	allocate_graphic_side_objects(UINT *handles);
void	desallocate_graphic_side_objects(UINT *handles);

#endif
