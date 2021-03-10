/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:46:02 by overetou          #+#    #+#             */
/*   Updated: 2021/03/10 13:46:04 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <fcntl.h>

void	handle_smooth_transition(t_master *render_data, GLuint time)
{
	if (render_data->transition_time_marker + 5000 > time)
	{
		render_data->transition_state = 1.0f /
		(5000.0f / (float)(time - render_data->transition_time_marker));
		if (render_data->direction == 0)
			render_data->transition_state = 1 - render_data->transition_state;
	}
	else
	{
		render_data->transition_state = (float)(render_data->direction);
		render_data->transition_time_marker = 0;
	}
}

static void	error_check_sdl(char val)
{
	if (val)
		return ;
	puts(SDL_GetError());
	exit(0);
}

void		error_check(char val, const char *msg)
{
	if (val)
		return ;
	puts(msg);
	exit(0);
}

void	handle_events_part_one(SDL_Event *e, char *params, t_master *m)
{
	if (e->key.keysym.sym == SDLK_ESCAPE)
		*params = 0;
	else if (e->key.keysym.sym == SDLK_w)
	{
		if (params[1])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			gl_check_errors("glPolygonMode");
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			gl_check_errors("glPolygonMode 2");
		}
		params[1] = !(params[1]);
	}
	else if (e->key.keysym.sym == SDLK_t && m->transition_time_marker == 0)
	{
		if (m->direction)
			m->direction = 0;
		else
			m->direction = 1;
		m->transition_time_marker = SDL_GetTicks();
	}
}

void	handle_events_part_two(SDL_Event *e, t_master *m)
{
	if (e->key.keysym.sym == SDLK_c)
		fill_vec3(m->rotation_axis, 0.0, 1.0, 0.0);
	else if (e->key.keysym.sym == SDLK_p)
		fill_vec3(m->rotation_axis, 0.0, 0.0, 1.0);
	else if (e->key.keysym.sym == SDLK_f)
		fill_vec3(m->rotation_axis, 1.0, 0.0, 0.0);
	else if (e->key.keysym.sym == SDLK_m)
		fill_vec3(m->rotation_axis, 0.1, 1.0, 1.0);
	else if (e->key.keysym.sym == SDLK_LEFT)
	{
		if (m->relative_coordinates[0] > -2)
			m->relative_coordinates[0] -= 0.1;
	}
	else if (e->key.keysym.sym == SDLK_RIGHT)
	{
		if (m->relative_coordinates[0] < 2)
			m->relative_coordinates[0] += 0.1;
	}
	else if (e->key.keysym.sym == SDLK_DOWN)
	{
		if (m->relative_coordinates[1] > -2)
			m->relative_coordinates[1] -= 0.1;
	}
}

void	handle_events_part_three(SDL_Event *e, t_master *m)
{
	if (e->key.keysym.sym == SDLK_UP)
	{
		if (m->relative_coordinates[1] < 2)
			m->relative_coordinates[1] += 0.1;
	}
	else if (e->key.keysym.sym == SDLK_MINUS)
	{
		if (m->relative_coordinates[2] > -7)
			m->relative_coordinates[2] -= 0.5;
	}
	else if (e->key.keysym.sym == SDLK_z)
	{
		if (m->relative_coordinates[2] < -2)
			m->relative_coordinates[2] += 0.5;
	}
}

void handle_events(char *params, t_master *m)
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				*params = 0;
				break;
			}
		}
		else if (e.type == SDL_KEYDOWN)
		{
			handle_events_part_one(&e, params, m);
			handle_events_part_two(&e, m);
			handle_events_part_three(&e, m);
			break;
		}
	}
}

GLfloat	square(GLfloat f)
{
	return (f * f);
}

void	normalize(GLfloat *vec)
{
	GLfloat w = sqrt( vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] );
	vec[0] /= w;
	vec[1] /= w;
	vec[2] /= w;
}

void	rotation_mat4(GLfloat *mat4, GLfloat radian_angle, GLfloat *axis)
{
	float angle_rad = radian_angle * (3.14159265359/180.0f);
	float c;
	float s;
	float t;

	normalize(axis);
	c = cos(angle_rad);
	s = sin(angle_rad);
	t = 1 - c;
	mat4[0] = c + axis[0] * axis[0] * t;
	mat4[1] = axis[1] * axis[0] * t + axis[2] * s;
	mat4[2] = axis[2] * axis[0] * t-axis[1] * s;
	mat4[3] = 0;
	mat4[4] = axis[0] * axis[1] * t - axis[2] * s;
	mat4[5] = c + axis[1] *axis[1] * t;
	mat4[6] = axis[2] * axis[1] * t+axis[0] * s;
	mat4[7] = 0;
	mat4[8] = axis[0] * axis[2] * t + axis[1] * s;
	mat4[9] = axis[1] * axis[2] * t - axis[0] * s;
	mat4[10] = axis[2] * axis[2] * t + c;
	mat4[11] = 0;
	mat4[12] = 0;
	mat4[13] = 0;
	mat4[14] = 0;
	mat4[15] = 1;
}

static void render_frame(UINT *handles, UINT texture, size_t len, t_master *m)
{
	GLfloat	home_model[16];
	GLfloat	*home_view;
	GLfloat	*home_proj;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_check_errors("glClear");
	glBindTexture(GL_TEXTURE_2D, texture);
	gl_check_errors("glBindTexture");
	rotation_mat4(home_model, ((float)SDL_GetTicks() / 1000) * 50.0f, m->rotation_axis);
	home_view = translation_mat4(m->relative_coordinates[0], m->relative_coordinates[1], m->relative_coordinates[2]);
	home_proj = perspective_mat4((float)800 / (float)600, degrees_to_radians(45), 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "model"), 1, GL_FALSE, home_model);
	gl_check_errors("glUniformMatrix4fv 1");
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "view"), 1, GL_FALSE, home_view);
	gl_check_errors("glUniformMatrix4fv 2");
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "projection"), 1, GL_FALSE, home_proj);
	gl_check_errors("glUniformMatrix4fv 3");
	gl_check_errors("glGetUniformLocation");
	glUniform1f(glGetUniformLocation(handles[3], "transition_degree"), m->transition_state);
	gl_check_errors("glUniform1f");
	glDrawArrays(GL_TRIANGLES, 0, len);
	gl_check_errors("glDrawArrays");
	free(home_view);
	free(home_proj);
}

unsigned char	*load_bmp(const char *file_path, int *width, int *height)
{
	unsigned char	header[54];
	size_t			dataPos;
	unsigned char	*data;
	int				fd;

	fd = open(file_path, O_RDONLY);
	dataPos = 0;
	error_check(fd > 0, "Could not open the bmp file.");
	error_check(read(fd, header, 54) == 54, "The file is not big enough to be formatted with bmp standards.");
	error_check(header[0] == 'B' && header[1] == 'M', "BMP file should begin with BM");
	dataPos = *(int*)&(header[0x0A]);
	*width= *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);
	if (dataPos==0)
		dataPos=54;
	lseek(fd, dataPos, SEEK_SET);
	data = (unsigned char*)secure_malloc(*(int*)&(header[0x22]));
	error_check(read(fd, data, *(int*)&(header[0x22])) == *(int*)&(header[0x22]), "Could not read the size specified in the bmp file header.");
	close(fd);
	return (data);
}

/*
**handles: 0-raw_vertices_data_handle, 1-vertex_shader_handle, 2-fragment
**_shader_handle, 3-shader_program, 4-vertexArrayObject	5-EBO
**Those handles are handles on the graphic card memory.
**params[0] = keep_going
**params[1] = wireframe mode enabled
*/
void init_render(t_master *m)
{
	m->transition_time_marker = 0;
	m->transition_state = 0;
	m->direction = 0;
	UINT handles[6];
	char params[2];

	params[0] = 1;
	params[1] = 0;
	fill_vec3(m->rotation_axis, 0.0, 1.0, 0.0);
	m->relative_coordinates[0] = 0;
	m->relative_coordinates[1] = 0;
	m->relative_coordinates[2] = -3;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gl_check_errors("glViewport");
	glClearColor(0.3, 0.3, 0.4, 1);
	gl_check_errors("glClearColor");
	size_t len = allocate_graphic_side_objects(handles, m);
	
	unsigned int texture;
	glGenTextures(1, &texture);
	gl_check_errors("glGenTextures");
	glActiveTexture(GL_TEXTURE0);
	gl_check_errors("glActiveTexture");
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	gl_check_errors("glBindTextures");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	gl_check_errors("glTexParameteri 1");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gl_check_errors("glTexParameteri 2");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl_check_errors("glTexParameteri 3");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl_check_errors("glTexParameteri 4");
	int width, height;
	unsigned char *data = load_bmp(m->text_file_path, &width, &height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	gl_check_errors("glTexImage2D");
	glGenerateMipmap(GL_TEXTURE_2D);
	gl_check_errors("glGenerateMipmap");
	free(data);
	glEnable(GL_DEPTH_TEST);
	gl_check_errors("glEnable GL_DEPTH_TEST");
	while (params[0]) 
	{
		if (m->transition_time_marker)
			handle_smooth_transition(m, SDL_GetTicks());
		render_frame(handles, texture, len, m);
		SDL_GL_SwapWindow(m->win);
		handle_events(params, m);
	}
	desallocate_graphic_side_objects(handles);
}

int			main(int argc, char const *argv[])
{
	t_master		m;
	SDL_GLContext	glcontext;

	error_check_sdl(SDL_Init(SDL_INIT_VIDEO) >= 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
	SDL_GL_CONTEXT_PROFILE_CORE) == 0);
	error_check_sdl(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);
	m.win = SDL_CreateWindow("Scop", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED,	WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	error_check_sdl(m.win != NULL);
	glcontext = SDL_GL_CreateContext(m.win);
	error_check_sdl(glcontext != NULL);
	SDL_GL_MakeCurrent(m.win, glcontext);
	error_check_sdl(SDL_GL_SetSwapInterval(1) == 0);
	error_check(glewInit() == GLEW_OK, "Failed to load OpenGL Functions.");
	process_args(argc, argv, &m);
	init_render(&m);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(m.win);
	SDL_Quit();
	return (0);
}
