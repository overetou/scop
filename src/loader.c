/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:48:57 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 17:52:11 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION
#include "scop.h"
#include <unistd.h>

unsigned int	get_file_size(int fd)
{
	int	result;

	result = lseek(fd, 0L, SEEK_END);
	if (result != -1)
	{
		lseek(fd, 0, SEEK_SET);
		return (result);
	}
	puts("Error while trying to reach obj file end.");
	exit(0);
}

void			handle_events_part_three(SDL_Event *e, t_master *m)
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

void			handle_events(char *params, t_master *m)
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				*params = 0;
				break ;
			}
		}
		else if (e.type == SDL_KEYDOWN)
		{
			handle_events_part_one(&e, params, m);
			handle_events_part_two(&e, m);
			handle_events_part_three(&e, m);
			break ;
		}
	}
}

GLfloat			square(GLfloat f)
{
	return (f * f);
}

void			normalize(GLfloat *vec)
{
	GLfloat w;

	w = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] /= w;
	vec[1] /= w;
	vec[2] /= w;
}
