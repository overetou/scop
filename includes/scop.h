
#ifndef SCOP_H
# define SCOP_H

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <math.h>
# include <stdio.h>
# include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define UINT unsigned int
#define VERTEX_SHADER_SOURCE "#version 440 core\n"\
"layout (location = 0) in vec3 aPos;"\
"layout (location = 1) in vec3 aColor;"\
"layout (location = 2) in vec2 aTexCoord;"\
"uniform mat4 model;"\
"uniform mat4 view;"\
"uniform mat4 projection;"\
"out vec3 ourColor;"\
"out vec2 TexCoord;"\
"void main()"\
"{"\
"	gl_Position = projection * view * model * vec4(aPos, 1.0);"\
"	ourColor = aColor;"\
"	TexCoord = aTexCoord;"\
"}"

#define FRAGMENT_SHADER_SOURCE "#version 440 core\n"\
"in vec3 ourColor;"\
"in vec2 TexCoord;"\
"out vec4 FragColor;"\
"uniform sampler2D ourTexture;"\
"void main()"\
"{"\
"    FragColor = texture(ourTexture, TexCoord);"\
"}"
#define USAGE "Wrong number of arguments.\nUsage:\t./scop"\
" <file>.obj <file>.bmp\n\tOr\n\t./scop [specs /"\
" vertex_shader_source / fragment_shader_source]"
//"uniform vec4 fixedColor;"

void	process_args(const int argc, const char *argv[]);
void	error_check(char val, const char *msg);
void	error_check_gl(GLenum error);
void	allocate_graphic_side_objects(UINT *handles);
void	desallocate_graphic_side_objects(UINT *handles);

#endif
