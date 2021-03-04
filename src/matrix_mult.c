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

GLfloat	mat4_mult_inner_compute(GLfloat *mat1, GLfloat *mat2,
int cur_row, int cur_col)
{
	return (
		mat1[ogl_pos(cur_row, 0)] * mat2[ogl_pos(0, cur_col)]
		+ mat1[ogl_pos(cur_row, 1)] * mat2[ogl_pos(1, cur_col)]
		+ mat1[ogl_pos(cur_row, 2)] * mat2[ogl_pos(2, cur_col)]
		+ mat1[ogl_pos(cur_row, 3)] * mat2[ogl_pos(3, cur_col)]);
}

void	multiply_mat4(GLfloat *mat1, GLfloat *mat2, GLfloat *result)
{
	int	cur_col;
	int	cur_row;

	cur_row = 0;
	while (cur_row != 4)
	{
		cur_col = 0;
		while (cur_col != 4)
		{
			result[ogl_pos(cur_row, cur_col)] =
			mat4_mult_inner_compute(mat1, mat2, cur_row, cur_col);
			cur_col++;
		}
		cur_row++;
	}
}

void	check_bmp_file(int fd, int *n, unsigned char *header)
{
	check_open(fd);
	check_file_format(n, header);
}
