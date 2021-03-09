/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_mult.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 13:49:38 by overetou          #+#    #+#             */
/*   Updated: 2020/06/23 17:28:29 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

GLfloat	*scaling_mat4(GLfloat x_multiplier, GLfloat y_multiplier,
GLfloat z_multiplier)
{
	GLfloat	*result;

	result = identity_mat4();
	result[0] = x_multiplier;
	result[5] = y_multiplier;
	result[10] = z_multiplier;
	return (result);
}

int		ogl_pos(int row, int col)
{
	return (col * 4 + row);
}
