/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 17:56:43 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 18:00:07 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <math.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600
# define VERTEX_SHADER_SOURCE "#version 440 core\n"\
"layout (location = 0) in vec3 aPos;\n"\
"layout (location = 1) in vec2 aTexCoord;\n"\
"layout (location = 2) in vec3 colors;\n"\
"uniform mat4 model;\n"\
"uniform mat4 view;\n"\
"uniform mat4 projection;\n"\
"out vec2 TexCoord;\n"\
"out vec3 frag_colors;\n"\
"void main()\n"\
"{\n"\
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"\
"	TexCoord = aTexCoord;\n"\
"	frag_colors = colors;\n"\
"}\n"

#define FRAGMENT_SHADER_SOURCE "#version 440 core\n"\
"in vec2 TexCoord;\n"\
"in flat vec3 frag_colors;\n"\
"out vec4 final_color;\n"\
"uniform sampler2D ourTexture;\n"\
"uniform float transition_degree;\n"\
"void main()\n"\
"{\n"\
"    final_color = vec4(texture(ourTexture, TexCoord).rgb * transition_degree +"\
"    frag_colors * (1 - transition_degree), 1);\n"\
"}\n"

#define USAGE "Wrong number of arguments.\nUsage:\t./scop"\
" <file>.obj <file>.bmp\n\tOr\n\t./scop [specs /"\
" vertex_shader_source / fragment_shader_source]"

/* "    FragColor = texture(ourTexture, TexCoord);\n"\ */

typedef struct		s_master
{
	SDL_Window		*win;
	const char		*obj_file_path;
	const char		*text_file_path;
	GLuint			transition_time_marker;
	GLfloat			transition_state;
	char			direction;
	GLfloat			rotation_axis[3];
	GLfloat			relative_coordinates[3];
	size_t			len;
	int				width;
	int				height;
	unsigned char	*data;
	GLuint			texture;
	size_t			vertices_len;
	GLfloat 		*home_vertices;
	GLfloat			*home_view;
	GLfloat			*home_proj;
}					t_master;

typedef struct	s_parsing_storage
{
	size_t		file_size;
	size_t		vert_size;
	size_t		final_vert_size;
	size_t		len_vts;
	size_t		i;
	GLfloat		*vertices;
	GLfloat		*final_vertices;
	GLfloat		*vts;
	char		*file_content;
	GLfloat		shaker;
}				t_parsing_storage;

void			process_args(const int argc, const char *argv[], t_master *m);
void			error_check(char val, const char *msg);
size_t			allocate_graphic_side_objects(unsigned int *handles, t_master *m);
void			desallocate_graphic_side_objects(unsigned int *handles);
GLfloat			*identity_mat4(void);
unsigned int	get_file_size(int fd);
GLfloat			vector_length(GLfloat *vec3);
void			cross_product(GLfloat *vec3_1, GLfloat *vec3_2, GLfloat *result);
void			fill_vec3(GLfloat *vec, GLfloat val1, GLfloat val2, GLfloat val3);
GLfloat			dot_product(GLfloat *vec3_1, GLfloat *vec3_2);
void			free_three(void *adr1, void *adr2, void *adr3);
void			check_file_format(int *n, unsigned char *header);
void			normalize_vec3(GLfloat *source, GLfloat *result);
GLfloat			*translation_mat4(GLfloat t_x, GLfloat t_y, GLfloat t_z);
GLfloat			*perspective_mat4(GLfloat aspect_ratio,
GLfloat vertical_field_of_view, GLfloat near_limit, GLfloat far_limit);
void			normalize_obj(GLfloat *vertices, size_t vert_size);
void			assign_max_min_initial_values(GLfloat *tab, GLfloat val);
void			determine_tab_diff(GLfloat *tab);
void			affect_diffs(GLfloat *vertices_sub_tab,
GLfloat			diff1, GLfloat diff2, GLfloat diff3);
void			do_for_each(GLfloat *tab1, GLfloat *tab2,
GLfloat *tab3, void (*func)(GLfloat *tab));
GLfloat			degrees_to_radians(GLfloat degrees);
void			gl_check_errors(const char *msg);
void			*secure_malloc(const size_t len);
void			change_shaker(t_parsing_storage *st);
void			glfloat_cpy_n(GLfloat *dest, const GLfloat *src, size_t n);
void 			compile_shader(GLenum shader_type, unsigned int *shader_handle,
const char 		*shader_source);
void 			check_compilation_step_success(unsigned int handle,
void (*getter)(unsigned int, GLenum, int*), GLenum status);

#endif
