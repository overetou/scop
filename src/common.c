/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:47:56 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 17:51:01 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	*secure_realloc(void *to_realloc, size_t new_size)
{
	void	*temp;

	temp = to_realloc;
	to_realloc = realloc(to_realloc, new_size);
	if (to_realloc)
		return (to_realloc);
	free(temp);
	puts("A realloc failed.");
	exit(0);
}

void	*secure_malloc(const size_t len)
{
	void *res;

	res = malloc(len);
	if (res)
		return (res);
	puts("A malloc failed.");
	exit(0);
}

void	rotation_mat4(GLfloat *mat4, GLfloat radian_angle, GLfloat *axis)
{
	float angle_rad;
	float c;
	float s;
	float t;

	angle_rad = radian_angle * (3.14159265359 / 180.0f);
	normalize(axis);
	c = cos(angle_rad);
	s = sin(angle_rad);
	t = 1 - c;
	mat4[0] = c + axis[0] * axis[0] * t;
	mat4[1] = axis[1] * axis[0] * t + axis[2] * s;
	mat4[2] = axis[2] * axis[0] * t - axis[1] * s;
	mat4[3] = 0;
	mat4[4] = axis[0] * axis[1] * t - axis[2] * s;
	mat4[5] = c + axis[1] * axis[1] * t;
	mat4[6] = axis[2] * axis[1] * t + axis[0] * s;
	mat4[7] = 0;
	mat4[8] = axis[0] * axis[2] * t + axis[1] * s;
	mat4[9] = axis[1] * axis[2] * t - axis[0] * s;
	mat4[10] = axis[2] * axis[2] * t + c;
	bzero(mat4 + 11, 4 * sizeof(GLfloat));
	mat4[15] = 1;
}

void	render_frame_part_one(GLfloat *home_model, UINT texture, t_master *m)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_check_errors("glClear");
	glBindTexture(GL_TEXTURE_2D, texture);
	gl_check_errors("glBindTexture");
	rotation_mat4(home_model, ((float)SDL_GetTicks() / 1000) *
	50.0f, m->rotation_axis);
	m->home_view = translation_mat4(m->relative_coordinates[0],
	m->relative_coordinates[1], m->relative_coordinates[2]);
	m->home_proj = perspective_mat4((float)800 / (float)600,
	degrees_to_radians(45), 0.1f, 100.0f);
}
