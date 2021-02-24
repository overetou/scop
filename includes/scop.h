
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
"layout (location = 1) in vec2 aTexCoord;"\
"uniform mat4 model;"\
"uniform mat4 view;"\
"uniform mat4 projection;"\
"out vec2 TexCoord;"\
"void main()"\
"{"\
"	gl_Position = projection * view * model * vec4(aPos, 1.0);"\
"	TexCoord = aTexCoord;"\
"}"

#define FRAGMENT_SHADER_SOURCE "#version 440 core\n"\
"in vec2 TexCoord;"\
"out vec4 FragColor;"\
"uniform sampler2D ourTexture;"\
"void main()"\
"{"\
"    FragColor = vec4(1, 1, 1, 1);"\
"}"
//"    FragColor = texture(ourTexture, TexCoord);"
#define USAGE "\nUsage:\t./scop"\
" <file>.obj <file>.bmp\n\tOr\n\t./scop [specs /"\
" vertex_shader_source / fragment_shader_source]"
//"uniform vec4 fixedColor;"

typedef struct	s_parsing_storage
{
	GLfloat		*vert;
	GLuint		v_nb;
	GLuint		*v_ind;
	GLuint		v_ind_size;
}				t_parsing_storage;

const char	*process_args(const int argc, const char *argv[]);
void	error_check(char val, const char *msg);
void	error_check_gl(GLenum error);
void	allocate_graphic_side_objects(UINT *handles, const char *path);
void	desallocate_graphic_side_objects(UINT *handles);
void	do_obj_parsing(int fd, t_parsing_storage *storage, unsigned int pos);
int		count_sep_till_endline(const char *s, int *till_next_line);
void	try_four_face_parsing(const char *content,
unsigned int *pos, t_parsing_storage *storage);
void	try_three_face_parsing(const char *content,
unsigned int *pos, t_parsing_storage *storage);
int		size_till_endline(const char *str);
void	check_vertices_number(t_parsing_storage *storage);
void	assign_max_min_initial_values(GLfloat *tab, GLfloat val);
void	do_for_each(GLfloat *tab1, GLfloat *tab2,
GLfloat *tab3, void (*func)(GLfloat *tab));
void	determine_tab_diff(GLfloat *tab);
void	affect_diffs(GLfloat *vertices_sub_tab,
GLfloat diff1, GLfloat diff2, GLfloat diff3);
void	normalize_obj(t_parsing_storage *storage);
GLfloat	*load_vertex_data_from_storage(t_parsing_storage *store,
GLfloat **color_data, GLfloat **uv_data);

#endif
