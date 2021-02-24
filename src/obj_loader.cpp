#include "scop.h"
#include <fcntl.h>
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