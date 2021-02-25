/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: overetou <overetou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 13:46:27 by overetou          #+#    #+#             */
/*   Updated: 2020/12/14 17:36:28 by overetou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <fcntl.h>
#include <unistd.h>

void	do_obj_parsing(int fd, t_parsing_storage *storage, unsigned int pos)
{
	unsigned int	file_size;
	char			*content;
	char			header[3];

	file_size = get_file_size(fd);
	content = (char*)malloc(file_size + 1);
	content[file_size] = '\0';
	bzero(storage, sizeof(t_parsing_storage));
	if (read(fd, content, file_size) > 0)
	{
		while (pos < file_size)
		{
			if (sscanf(content + pos, "%2s", header) == 1)
				try_line_parsing(header, content, &pos, storage);
			if (content[pos] == '\n')
				pos++;
		}
		free(content);
		close(fd);
	}
	else
	{
		puts("Error : empty or corrupted obj file.");
		exit(0);
	}
}

void	check_open(int fd)
{
	if (fd == -1)
	{
		puts("Could not open file with the given path. Is your texture file located in the current dir?");
		exit(0);
	}
}

void	check_file_format(int *n, unsigned char *header)
{
	if (read(n[4], header, 54) != 54 || header[0] != 'B' || header[1] != 'M')
	{
		puts("Could not read bmp file header.");
		exit(0);
	}
}

void	check_image_correct_read(int *n, char *image)
{
	if (read(n[4], image, n[3]) != n[3])
	{
		puts("Could not read image data in coherence with bmp header.");
		exit(0);
	}
}

/*
**n[0] = data_pos, n[1] = width, n[2] = height, n[3] = image_size, n[4] = fd
*/

void	load_bmp_from_filename(const char *path)
{
	unsigned char	header[54];
	int				n[5];
	char			*image;
	GLuint			texture_name;

	n[4] = open(path, O_RDONLY);
	check_bmp_file(n[4], n, header);
	n[0] = *(int*)&(header[0x0A]);
	n[3] = *(int*)&(header[0x22]);
	n[1] = *(int*)&(header[0x12]);
	n[2] = *(int*)&(header[0x16]);
	if (n[3] == 0)
		n[3] = n[1] * n[2];
	if (n[0] == 0)
		n[0] = 54;
	image = (char*)malloc(sizeof(char) * n[3]);
	check_image_correct_read(n, image);
	close(n[4]);
	glGenTextures(1, &texture_name);
	glBindTexture(GL_TEXTURE_2D, texture_name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n[1], n[2], 0, GL_BGR,
				GL_UNSIGNED_BYTE, image);
	free(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
