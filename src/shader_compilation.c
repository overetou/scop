/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_compilation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 14:28:03 by overetou          #+#    #+#             */
/*   Updated: 2021/03/11 17:17:43 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <scop.h>

void	check_compilation_step_success(GLuint handle,
void (*getter)(GLuint, GLenum, int*), GLenum status)
{
	int		success;
	char	info_log[512];

	getter(handle, status, &success);
	if (success)
		return ;
	glGetShaderInfoLog(handle, 512, NULL, info_log);
	printf("Shader compilation failed: %s\n", info_log);
	exit(0);
}

void	compile_shader(GLenum shader_type, GLuint *shader_handle,
const char *shader_source)
{
	*shader_handle = glCreateShader(shader_type);
	glShaderSource(*shader_handle, 1, &shader_source, NULL);
	glCompileShader(*shader_handle);
	check_compilation_step_success(*shader_handle, glGetShaderiv,
	GL_COMPILE_STATUS);
}

void	glfloat_cpy_n(GLfloat *dest, const GLfloat *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i != n)
	{
		dest[i] = src[i];
		i++;
	}
}

void	change_shaker(t_parsing_storage *st)
{
	if (st->shaker < 0.55)
		st->shaker += 0.02;
	else
		st->shaker = 0.25;
}
