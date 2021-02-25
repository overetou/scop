/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_basics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 13:49:30 by overetou          #+#    #+#             */
/*   Updated: 2020/06/23 17:18:07 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	vector_substraction(GLfloat *vec3_1, GLfloat *vec3_2, GLfloat *result)
{
	result[0] = vec3_1[0] - vec3_2[0];
	result[1] = vec3_1[1] - vec3_2[1];
	result[2] = vec3_1[2] - vec3_2[2];
}

void	normalize_vec3(GLfloat *source, GLfloat *result)
{
	GLfloat	vec_length;

	vec_length = vector_length(source);
	result[0] = source[0] / vec_length;
	result[1] = source[1] / vec_length;
	result[2] = source[2] / vec_length;
}

GLfloat	*look_at_mat4(GLfloat *cam_pos, GLfloat *focus_pos, GLfloat *up_pos)
{
	GLfloat	z_axis[3];
	GLfloat	x_axis[3];
	GLfloat	y_axis[3];
	GLfloat	temp[3];
	GLfloat	*result;

	result = (GLfloat*)malloc(16 * sizeof(GLfloat));
	vector_substraction(focus_pos, cam_pos, temp);
	normalize_vec3(temp, z_axis);
	normalize_vec3(up_pos, y_axis);
	cross_product(z_axis, y_axis, temp);
	normalize_vec3(temp, x_axis);
	cross_product(x_axis, z_axis, y_axis);
	fill_vec3(result, x_axis[0], y_axis[0], -z_axis[0]);
	result[3] = 0;
	fill_vec3(result + 4, x_axis[1], y_axis[1], -z_axis[1]);
	result[7] = 0;
	fill_vec3(result + 8, x_axis[2], y_axis[2], -z_axis[2]);
	result[12] = 0;
	fill_vec3(result + 12, -dot_product(x_axis, cam_pos),
	-dot_product(y_axis, cam_pos), dot_product(z_axis, cam_pos));
	result[15] = 1;
	free_three(cam_pos, focus_pos, up_pos);
	return (result);
}

GLfloat	*perspective_mat4(GLfloat aspect_ratio,
GLfloat vertical_field_of_view, GLfloat near_limit, GLfloat far_limit)
{
	GLfloat	*result;
	GLfloat	tan_half_angle;

	result = (GLfloat*)malloc(16 * sizeof(GLfloat));
	tan_half_angle = tan(vertical_field_of_view / 2);
	fill_vec3(result, 1 / (aspect_ratio * tan_half_angle), 0, 0);
	result[3] = 0;
	fill_vec3(result + 4, 0, 1 / tan_half_angle, 0);
	result[7] = 0;
	fill_vec3(result + 8, 0, 0, -(far_limit + near_limit) /
	(far_limit - near_limit));
	result[11] = -1;
	fill_vec3(result + 12, 0, 0, -(2 * far_limit * near_limit) /
	(far_limit - near_limit));
	result[15] = 0;
	return (result);
}

GLfloat	degrees_to_radians(GLfloat degrees)
{
	return (degrees * M_PI / 180);
}
