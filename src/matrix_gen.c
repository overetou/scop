/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_gen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 13:49:22 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 17:54:40 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

GLfloat	*identity_mat4(void)
{
	GLfloat	*result;

	result = (GLfloat*)secure_malloc(16 * sizeof(GLfloat));
	fill_vec3(result, 1, 0, 0);
	result[3] = 0;
	fill_vec3(result + 4, 0, 1, 0);
	result[7] = 0;
	fill_vec3(result + 8, 0, 0, 1);
	result[11] = 0;
	fill_vec3(result + 12, 0, 0, 0);
	result[15] = 1;
	return (result);
}

GLfloat	*translation_mat4(GLfloat t_x, GLfloat t_y, GLfloat t_z)
{
	GLfloat	*result;

	result = identity_mat4();
	result[12] = t_x;
	result[13] = t_y;
	result[14] = t_z;
	return (result);
}

void	error_check(char val, const char *msg)
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
