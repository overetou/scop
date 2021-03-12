/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:46:02 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 17:54:17 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <fcntl.h>

void			render_frame(GLuint *handles, GLuint texture,
size_t len, t_master *m)
{
	GLfloat	home_model[16];

	render_frame_part_one(home_model, texture, m);
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "model"), 1, GL_FALSE,
	home_model);
	gl_check_errors("glUniformMatrix4fv 1");
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "view"),
	1, GL_FALSE, m->home_view);
	gl_check_errors("glUniformMatrix4fv 2");
	glUniformMatrix4fv(glGetUniformLocation(handles[3], "projection"), 1,
	GL_FALSE, m->home_proj);
	gl_check_errors("glUniformMatrix4fv 3");
	gl_check_errors("glGetUniformLocation");
	glUniform1f(glGetUniformLocation(handles[3], "transition_degree"),
	m->transition_state);
	gl_check_errors("glUniform1f");
	glDrawArrays(GL_TRIANGLES, 0, len);
	gl_check_errors("glDrawArrays");
	free(m->home_view);
	free(m->home_proj);
}

unsigned char	*load_bmp(const char *file_path, int *width, int *height)
{
	unsigned char	header[54];
	size_t			datapos;
	unsigned char	*data;
	int				fd;

	fd = open(file_path, O_RDONLY);
	datapos = 0;
	error_check(fd > 0, "Could not open the bmp file.");
	error_check(read(fd, header, 54) == 54,
	"The file is not big enough to be formatted with bmp standards.");
	error_check(header[0] == 'B' && header[1] == 'M',
	"BMP file should begin with BM");
	datapos = *(int*)&(header[0x0A]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);
	if (datapos == 0)
		datapos = 54;
	lseek(fd, datapos, SEEK_SET);
	data = (unsigned char*)secure_malloc(*(int*)&(header[0x22]));
	error_check(read(fd, data, *(int*)&(header[0x22])) ==
	*(int*)&(header[0x22]),
	"Could not read the size specified in the bmp file header.");
	close(fd);
	return (data);
}

void			init_render_step_one(t_master *m, char *params, GLuint *handles)
{
	bzero(&(m->transition_time_marker),
	sizeof(GLuint) + sizeof(GLfloat) + sizeof(char));
	params[0] = 1;
	params[1] = 0;
	fill_vec3(m->rotation_axis, 0.0, 1.0, 0.0);
	fill_vec3(m->relative_coordinates, 0, 0, -3);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gl_check_errors("glViewport");
	glClearColor(0.3, 0.3, 0.4, 1);
	gl_check_errors("glClearColor");
	m->len = allocate_graphic_side_objects(handles, m);
	glGenTextures(1, &(m->texture));
	gl_check_errors("glGenTextures");
	glActiveTexture(GL_TEXTURE0);
	gl_check_errors("glActiveTexture");
	glBindTexture(GL_TEXTURE_2D, m->texture);
	gl_check_errors("glBindTextures");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	gl_check_errors("glTexParameteri 1");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gl_check_errors("glTexParameteri 2");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl_check_errors("glTexParameteri 3");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl_check_errors("glTexParameteri 4");
}

void			init_render(t_master *m)
{
	GLuint handles[6];
	char params[2];

	init_render_step_one(m, params, handles);
	m->data = load_bmp(m->text_file_path, &(m->width), &(m->height));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m->width,
	m->height, 0, GL_BGR, GL_UNSIGNED_BYTE, m->data);
	gl_check_errors("glTexImage2D");
	glGenerateMipmap(GL_TEXTURE_2D);
	gl_check_errors("glGenerateMipmap");
	free(m->data);
	glEnable(GL_DEPTH_TEST);
	gl_check_errors("glEnable GL_DEPTH_TEST");
	while (params[0])
	{
		if (m->transition_time_marker)
			handle_smooth_transition(m, SDL_GetTicks());
		render_frame(handles, m->texture, m->len, m);
		SDL_GL_SwapWindow(m->win);
		handle_events(params, m);
	}
	desallocate_graphic_side_objects(handles);
}

int				main(int argc, char const *argv[])
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
	SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
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
