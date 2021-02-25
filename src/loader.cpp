#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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

void	try_vertex_parsing(const char *content, unsigned int *pos,
t_parsing_storage *storage)
{
	int	item_parsed_number;

	storage->vert = (GLfloat*)realloc(storage->vert,
	(storage->v_nb + 3) * sizeof(GLfloat));
	if (storage->vert)
	{
		item_parsed_number = sscanf(content + *pos, "%f %f %f\n",
		storage->vert + storage->v_nb,
		storage->vert + storage->v_nb + 1,
		storage->vert + storage->v_nb + 2);
		if (item_parsed_number != 3)
		{
			puts("Parsed a wrong number of vertices.");
			exit(0);
		}
		storage->v_nb += 3;
	}
	else
	{
		puts("Realloc error.");
		exit(0);
	}
}

int			count_sep_till_endline(const char *s, int *till_next_line)
{
	int sep_number;
	int	i;

	i = 0;
	sep_number = 0;
	while (s[i] && s[i] != '\n')
	{
		while (s[i] != ' ')
		{
			i++;
			if (s[i] == '\0' || s[i] == '\n')
			{
				*till_next_line = i;
				return (sep_number + 1);
			}
		}
		if (i)
			sep_number++;
		while (s[i] == ' ')
			i++;
	}
	*till_next_line = i + 1;
	return (sep_number + 1);
}

void	try_four_face_parsing(const char *content,
unsigned int *pos, t_parsing_storage *storage)
{
	int	temp[4];

	if (sscanf(content + *pos, "%d %d %d %d\n", temp, temp + 1,
	temp + 2, temp + 3) != 4)
	{
		puts("Problem while scanning a face of 4 members.");
		exit(0);
	}
	storage->v_ind = (unsigned int*)realloc(storage->v_ind,
	(storage->v_ind_size + 6) * sizeof(int));
	storage->v_ind[storage->v_ind_size] = temp[0];
	storage->v_ind[storage->v_ind_size + 1] = temp[1];
	storage->v_ind[storage->v_ind_size + 2] = temp[2];
	storage->v_ind[storage->v_ind_size + 3] = temp[2];
	storage->v_ind[storage->v_ind_size + 4] = temp[3];
	storage->v_ind[storage->v_ind_size + 5] = temp[0];
	storage->v_ind_size += 6;
}

void	try_three_face_parsing(const char *content,
unsigned int *pos, t_parsing_storage *storage)
{
	int	item_parsed_number;

	storage->v_ind = (unsigned int*)realloc(storage->v_ind,
	(storage->v_ind_size + 3) * sizeof(int));
	item_parsed_number = sscanf(content + *pos, "%d %d %d\n",
	storage->v_ind + storage->v_ind_size,
	storage->v_ind + storage->v_ind_size + 1,
	storage->v_ind + storage->v_ind_size + 2);
	if (item_parsed_number != 3)
	{
		puts("Problem while scanning a face of 3 members.");
		exit(0);
	}
	storage->v_ind_size += 3;
}

void	try_face_parsing(const char *content, unsigned int *pos,
t_parsing_storage *storage)
{
	int sep_count;
	int till_next_line;

	sep_count = count_sep_till_endline(content + *pos, &till_next_line);
	if (sep_count == 3)
		try_three_face_parsing(content, pos, storage);
	else if (sep_count == 4)
		try_four_face_parsing(content, pos, storage);
	else
	{
		puts("Inadequate number of vertices detected in a face while parsing.");
		exit(0);
	}
}

int				size_till_endline(const char *str)
{
	int result;

	result = 0;
	while (str[result] != '\n' && str[result])
		result++;
	return (result);
}

void	try_line_parsing(const char *header, const char *content,
unsigned int *pos, t_parsing_storage *storage)
{
	*pos += strlen(header);
	if (strcmp(header, "v") == 0)
		try_vertex_parsing(content, pos, storage);
	else if (strcmp(header, "f") == 0)
	{
		try_face_parsing(content, pos, storage);
		return ;
	}
	*pos += size_till_endline(content + *pos) + 1;
}
