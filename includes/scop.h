
#ifndef SCOP_H
# define SCOP_H

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <math.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>

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
"    FragColor = texture(ourTexture, TexCoord);"\
"}"
#define USAGE "Wrong number of arguments.\nUsage:\t./scop"\
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

void	process_args(const int argc, const char *argv[]);
void	error_check(char val, const char *msg);
void	error_check_gl(GLenum error);
void	allocate_graphic_side_objects(UINT *handles);
void	desallocate_graphic_side_objects(UINT *handles);
GLfloat	*identity_mat4(void);
unsigned int	get_file_size(int fd);
void	try_line_parsing(const char *header, const char *content,
unsigned int *pos, t_parsing_storage *storage);
void	check_bmp_file(int fd, int *n, unsigned char *header);
GLfloat	vector_length(GLfloat *vec3);
void	cross_product(GLfloat *vec3_1, GLfloat *vec3_2, GLfloat *result);
void	fill_vec3(GLfloat *vec, GLfloat val1, GLfloat val2, GLfloat val3);
GLfloat	dot_product(GLfloat *vec3_1, GLfloat *vec3_2);
void	free_three(void *adr1, void *adr2, void *adr3);
void	check_open(int fd);
void	check_file_format(int *n, unsigned char *header);
void	normalize_vec3(GLfloat *source, GLfloat *result);
GLfloat	*translation_mat4(GLfloat t_x, GLfloat t_y, GLfloat t_z);
GLfloat	*perspective_mat4(GLfloat aspect_ratio,
GLfloat vertical_field_of_view, GLfloat near_limit, GLfloat far_limit);
void			load_obj(const char *path, t_parsing_storage *storage);
void	normalize_obj(t_parsing_storage *storage);
void	check_vertices_number(t_parsing_storage *storage);
void	assign_max_min_initial_values(GLfloat *tab, GLfloat val);
void	determine_tab_diff(GLfloat *tab);
void	affect_diffs(GLfloat *vertices_sub_tab,
GLfloat diff1, GLfloat diff2, GLfloat diff3);
void	do_for_each(GLfloat *tab1, GLfloat *tab2,
GLfloat *tab3, void (*func)(GLfloat *tab));
#endif
