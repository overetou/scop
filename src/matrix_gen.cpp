/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_gen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 13:49:22 by overetou          #+#    #+#             */
/*   Updated: 2020/06/23 17:23:35 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

GLfloat	*identity_mat4(void)
{
	GLfloat	*result;

	result = (GLfloat*)malloc(16 * sizeof(GLfloat));
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

GLfloat	*x_rotation_mat4(GLfloat angle)
{
	GLfloat	*result;

	result = identity_mat4();
	result[5] = cos(angle);
	result[6] = -sin(angle);
	result[9] = sin(angle);
	result[9] = cos(angle);
	return (result);
}

GLfloat	*y_rotation_mat4(GLfloat angle)
{
	GLfloat	*result;

	result = identity_mat4();
	result[0] = cos(angle);
	result[2] = sin(angle);
	result[8] = -sin(angle);
	result[10] = cos(angle);
	return (result);
}

GLfloat	*z_rotation_mat4(GLfloat angle)
{
	GLfloat	*result;

	result = identity_mat4();
	result[0] = cos(angle);
	result[1] = sin(angle);
	result[4] = -sin(angle);
	result[5] = cos(angle);
	return (result);
}
