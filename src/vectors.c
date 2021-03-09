/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 13:50:12 by overetou          #+#    #+#             */
/*   Updated: 2020/06/23 17:49:52 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	fill_vec3(GLfloat *vec, GLfloat val1, GLfloat val2, GLfloat val3)
{
	vec[0] = val1;
	vec[1] = val2;
	vec[2] = val3;
}

GLfloat	*create_vec3(GLfloat val1, GLfloat val2, GLfloat val3)
{
	GLfloat	*result;

	result = (GLfloat*)secure_malloc(3 * sizeof(GLfloat));
	fill_vec3(result, val1, val2, val3);
	return (result);
}

GLfloat	dot_product(GLfloat *vec3_1, GLfloat *vec3_2)
{
	GLfloat	result;

	result = vec3_1[0] * vec3_2[0];
	result += vec3_1[1] * vec3_2[1];
	result += vec3_1[2] * vec3_2[2];
	return (result);
}

void	cross_product(GLfloat *vec3_1, GLfloat *vec3_2, GLfloat *result)
{
	result[0] = vec3_1[1] * vec3_2[2] - vec3_1[2] * vec3_2[1];
	result[1] = vec3_1[2] * vec3_2[0] - vec3_1[0] * vec3_2[2];
	result[2] = vec3_1[0] * vec3_2[1] - vec3_1[1] * vec3_2[0];
}

GLfloat	vector_length(GLfloat *vec3)
{
	return (sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2)));
}
